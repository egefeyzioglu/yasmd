
# Tokens

; Longest match in the following order of preference
comment
`;.*` ; . does not include newline characters
identifier
`[a-zA-Z][a-zA-Z\-0-9]*`
define_inc
`=/`
define
`=`
slash
`/`
lparen
`\\(`
rparen
`\\)`
lbrack
`\\[`
rbrack
`\\]`
star
`\*`
string_literal ; "quoted string"
`"[\x20-\x22\x23-\x7e]*"`
num_value ; including the %
`%([bdx])([0-9a-fA-F]+)(([\-.]?)([0-9a-fA-F]*))*` ; Further validated after matching
prose_literal ; < angle brackets string >
`<[\x20-\x3D\x3F-\x7E]*>`
integer_literal
`[0-9]+`
newline
`\n|\r|\r\n`
whitespace
` |\t`
# Grammar
```
rulelist = 1*( rule / (*c-wsp c-nl))

rule = rulename defined-as elements c-nl

rulename = ALPHA *(ALPHA / DIGIT / "-")

defined-as = *c-wsp ("=" / "=/") *c-wsp
        ; "=" for definition
        ; "=/" for appending

elements = alternation *WSP

c-nl = comment / CRLF

c-wsp = WSP / (c-nl WSP)

alternation = concatenation
    *(*c-wsp "/" *c-wsp concatenation)


concatenation = repetition
    *(1*c-wsp repetition)

repetition = [repeat] element

repeat = 1*DIGIT / (*DIGIT "*" *DIGIT)

element = rulename / group / option / char-val / num-val / prose-val

group = "(" *c-wsp alternation *c-wsp ")"

option = "[" *c-wsp alternation *c-wsp "]"

char-val = DQUOTE *(%x20-21 / %x23-7E) DQUOTE

num-val = "%" (bin-val / dec-val / hex-val)

bin-val = "b" 1*BIT [1*("." 1*BIT) / ("-" 1*BIT)]
dec-val = "d" 1*DIGIT [1*("." 1*DIGIT) / ("-" 1*DIGIT)]
hex-val = "x" 1*HEXDIG [1*("." 1*HEXDIG) / ("-" 1*HEXDIG)]

prose-val = "<" *(%x20-3D / %x3F-7E) ">"


;;;;;;;;;;;;;;;;;;;;;;;

ALPHA          =  %x41-5A / %x61-7A   ; A-Z / a-z

BIT            =  "0" / "1"

CHAR           =  %x01-7F
                    ; any 7-bit US-ASCII character,
                    ;  excluding NUL

CR             =  %x0D
                    ; carriage return

CRLF           =  CR LF
                    ; Internet standard newline

CTL            =  %x00-1F / %x7F
                    ; controls

DIGIT          =  %x30-39
                    ; 0-9

DQUOTE         =  %x22
                    ; " (Double Quote)

HEXDIG         =  DIGIT / "A" / "B" / "C" / "D" / "E" / "F"

HTAB           =  %x09
                    ; horizontal tab

LF             =  %x0A
                    ; linefeed

LWSP           =  *(WSP / CRLF WSP)
                    ; Use of this linear-white-space rule
                    ;  permits lines containing only white
                    ;  space that are no longer legal in
                    ;  mail headers and have caused
                    ;  interoperability problems in other
                    ;  contexts.
                    ; Do not use when defining mail
                    ;  headers and use with caution in
                    ;  other contexts.

OCTET          =  %x00-FF
                    ; 8 bits of data

SP             =  %x20

VCHAR          =  %x21-7E
                    ; visible (printing) characters

WSP            =  SP / HTAB
                    ; white space
```