# GPP
> General PreProcessor

```html
<html>
    <body>
        <h2>I really want this list to be rendered using Python!</h2>
        <ul>
((#!/usr/bin/python
for name in ["john", "sarah", "anna"]:
    x = """((
        <li>
            {}     
        </li>
    ))""".format(name)
    print(x)
 ))
        </ul>

        <h2>I do want this list to be rendered in Node.js though</h2>
        <ul>
((#!/usr/bin/node
const result = ["john", "sarah", "anna"].map(name => "<li>" + name +  "</li>");
const html = result.join('\n');
console.log(html);
))
        </ul>
    </body>
</html>
```

## Templating
### The basics
> There are a few important tokens in the templating language,
> and we will go through them in this section.
#### {{ , }}
> The `{{` and the `}}` tokens are used to indicate the start `{{` and the end `}}`
> of a block of templating logic.
#### (( , ))
> The `((` and the `))` tokens are used to indicate the start `((` and the end `))`
> of a ignored block.
> And ignored block is something that is just treated as text by the preprocessor.
> In other words, nothing within these tokens will be interpreted.
> The entire document is by default wrapped in a ignored block.
##### One Exception
> There is one exception to the `(( ))` (ignore block) however,
> if you specify a path to an interpreter, the rest of the content will be
> interpreted. _(There is an example further down in this document)_

#### How the blocks work
> The following:  
```html
<p>{{ "hello" }}</p>
```
> Will be rendered as:
```html
<p>hello</p>
```
> The following:  
```html
<p>(( "hello" ))</p>
```
> Will be rendered as:
```html
<p> "hello" </p>
```
#### Practical Example
> Here is an example that shows the basics:
```html
<!--
    {{ }} = ** Template Logic Block ** 
          - A block that is interpreted as templating logic

    (( )) = ** Virtual File Block **
          - Anything in here is just treated as if it was a completely new file.

    ( ) = ** List **
          - A list of elements, separated by newline or space.
          - Aliases: [ ] 
!-->
<html>
    <body>
        <h1>{{ "My Headline" }}</h1>
         <p>{{ "It works" }}</p>
         <p>((It sure {{ "does" }}))</p>
         <span>((
         Here is some text that will be completely ignored.   
         This: <b>{{ "Is not ignored however" }}</b>.
         ))</span>
         <ul>
            {{
                (
                    ((<li>Here is a item</li>))
                    ((<li>Another item</li>))
                )
            }}
    </body>
</html>
```
#### Any interpreter you want
> If you do not want to use the default language,
> you can use any language you want to interpret the templates.
> Here is an example where we are rendering a `<ul>` list using `Python`.
```html
<html>
    <body>
        <ul>
((#!/usr/bin/python
for i in range(100):
    x = """((
        <li>
            {}     
        </li>
    ))""".format(i)
    print(x)
 ))
        </ul>
    </body>
</html>
```
> To make this work, we simply put a comment inside our `(( ))` block.
> This comment should start with a `#!` and then the path to the
> program that should be interpreting the rest of the content inside of the
> `(( ))` block.
