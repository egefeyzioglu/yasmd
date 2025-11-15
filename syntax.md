# Grammar
```
document = *node
node = paragraph | heading
paragraph = *char
heading = h1|h2|h3
h1 = "#" *char
h2 = "##" *char
h3 = "###" *char
```

# Token Types

* string_lit
* heading_start