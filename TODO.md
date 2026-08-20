# TODO

## Unambiguous sexpr-like syntax (for linearize / a future text format)

### The problem

prio's data model has three different compound shapes:

| Shape | Meaning |
|-------|---------|
| **Object** | A *named* node with a property mapping (`ReaderObject`) |
| **Mapping** | A bag of key → value pairs (`ReaderMapping`) |
| **Collection** | An ordered list of objects (`ReaderCollection`) |

Classic sexpr uses `(…)` for almost everything:

```sexpr
(server
  (host "example.com")
  (port 8080)
  (tls
    (enabled #t)
    (certificate "cert.pem"))
  (items
    (wall (texture "brick.png"))
    (floor (texture "wood.png"))))
```

Here `(tls …)` and `(items …)` look the same, but one is a nested
mapping of properties and the other is a collection of named objects.
Whether `(wall (texture …))` is “a mapping key `wall`” or “an object
named `wall`” is not visible in the parentheses alone — the reader
decides by *how* the value is requested (`read(key, mapping)` vs
`read(key, collection)` vs `read(key, object)`).

That is fine for the C++ API. It is not fine for a greppable
path = value dump, or for a text format that should be readable without
the API's intent:

```text
# Ambiguous with only '.' and '()' / indistinguishable segments:
server.tls.enabled = true
server.items.wall.texture = brick.png
```

Is `items` a mapping key or a collection? Is `wall` a key or an object
name? A path grammar that uses the same separator for every level cannot
say.

A previous idea was to mark object names with `@` in linearized output
(`server.items[0]@wall.texture`). That disambiguates the *dump*, but it
is an ad-hoc path dialect, not a real sexpr-like document syntax, and it
does not help the underlying “everything is a list” problem.

**Requirement:** any alternative should be unambiguous from the syntax
alone, even if that means **not** being plain sexpr anymore.

---

### Direction: one bracket kind per shape

Give each compound shape its own delimiters:

| Shape | Delimiters | Role of first element / content |
|-------|------------|----------------------------------|
| Object | `( name … )` | First symbol is the object name; body is a mapping |
| Mapping | `{ … }` | Key/value pairs only (no “name” slot) |
| Collection | `[ … ]` | Ordered sequence of objects (or, for scalar arrays, values) |

Plain sexpr compatibility is intentionally given up. The gain is that
shape is local and obvious to both humans and parsers.

#### Sketch (document form)

```text
(server {
  host "example.com"
  port 8080
  tls {
    enabled #t
    certificate "cert.pem"
  }
  items [
    (wall {
      texture "brick.png"
    })
    (floor {
      texture "wood.png"
    })
  ]
})
```

Reading rules (no API hint required):

- `(foo { … })` — object named `foo` whose properties are the mapping
- `{ key value … }` — mapping
- `[ … ]` — collection (elements should be objects) or scalar list
- atoms — bool / int / float / string as today

Homogeneous scalar arrays stay distinguishable by element type:

```text
flags [ #t #f #t ]
tags  [ "a" "b" ]
```

vs object collections:

```text
items [ (wall { … }) (floor { … }) ]
```

#### Linearize form derived from the same grammar

If the document form is unambiguous, a one-path-per-line dump can mirror
it without inventing a third dialect:

```text
(server){host} = example.com
(server){port} = 8080
(server){tls}{enabled} = true
(server){tls}{certificate} = cert.pem
(server){items}[0](wall){texture} = brick.png
(server){items}[1](floor){texture} = wood.png
(server){flags}[0] = true
```

Path segments:

- `(name)` — step into a named object
- `{key}` — step into a mapping key
- `[N]` — step into a collection / array index

No segment type is overloaded. Grep stays useful (`grep '{tls}'`,
`grep '(wall)'`, `grep '\[0\]'`).

---

### Alternative: keep `()` for objects+mappings, use `[]` only for collections

Slightly closer to today's sexpr (still mostly lists):

```text
(server
  (host "example.com")
  (items
    [ (wall (texture "brick.png"))
      (floor (texture "wood.png")) ]))
```

- `(name …)` remains “named list” (object or property tree)
- `[ … ]` is always a collection / array

**Limitation:** still does not distinguish “nested mapping under key
`tls`” from “nested object named `tls`” if both use `(tls …)`. So this
only fixes the collection case, not the object-vs-mapping case.

Prefer the three-way `( )` / `{ }` / `[ ]` split if the goal is full
unambiguity.

---

### Alternative: `{}` for mappings only, keep `()` for objects, collections as repeated objects under a key

Some formats (EDN-ish / JSON-ish hybrids) do:

```text
(server {
  items [
    (wall { texture "brick.png" })
  ]
})
```

Same as the main sketch. Collections must not be written as

```text
(items (wall …) (floor …))  ; BAD: looks like a mapping of properties
```

and must use `[ … ]` instead.

---

### Design constraints to keep in mind

1. **Object name is not a mapping key.**  
   `(wall { texture "…" })` — `wall` is the object type/name; `texture`
   is a property. Linearize must not turn that into a single `.` chain
   without a marker.

2. **Collections are ordered; mappings are not.**  
   Indices belong only on `[N]` steps. Do not invent synthetic keys
   `0`, `1` inside mappings.

3. **Scalar arrays vs object collections.**  
   Both may use `[]`, distinguished by whether elements are atoms or
   `(name {…})` forms. If that is ever ambiguous in practice, split
   scalar arrays into a dedicated form (e.g. `#(1 2 3)` or explicit
   tagged lists).

4. **Keys with special characters.**  
   Mapping keys that contain spaces, dots, or brackets need a quoting
   rule inside `{ }`, e.g. `{"odd.key" value}`.

5. **Round-trip vs grep-only.**  
   Linearize can be dump-only (no parser). A full document syntax should
   be parseable back into `ReaderDocument` if it is meant to replace
   sexpr as an input format.

6. **JSON already has the three shapes.**  
   JSON objects ≈ mappings (and single-key objects ≈ named objects only
   by prio convention), arrays ≈ collections. Any new sexpr-like form is
   mainly for the sexpr side and for a shared greppable view of both
   backends.

---

### Possible next steps

- [ ] Decide whether linearize stays dump-only or should match a real
      document grammar.
- [ ] Prototype the `( name { … } )` / `{ … }` / `[ … ]` writer (and
      optionally reader) beside the existing sexpr backend.
- [ ] Revisit `priotool --linearize` paths so segments are `(name)`,
      `{key}`, `[N]` instead of overloaded `.` separators.
- [ ] Document migration: old sexpr files remain supported; new syntax is
      opt-in (`--sexp3` / `Format::SEXP_STRUCTURED` or similar).

