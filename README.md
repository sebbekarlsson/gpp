# GPP
> General PreProcessor
> (Not implemented yet)

## Templating
### The basics
> There are a few important tokens in the templating language,
> and we will go through them in this section.
#### {{ , }}
> The `{{` and the `}}` tokens are used to indicate the start `{{` and the end `}}`
> of a block of templating logic.
#### (( , ))
> The `((` and the `))` tokens are used to indicate the start `((` and the end `))`
> of a ingored block.
> And ignored block is something that is just treated as text by the preprocessor.
> In other words, nothing within these tokens will be interpreted.
> The entire document is by default wrapped in a ignored block.
#### @
> The `@` (at sign) is a variable that is instantiated within every `{{` , `}}` block.
> Each block has their own unique `@` (at sign) variable.
> This at sign contains the content of what the `{{`, `}}` block will be equal to.
> In other words, whatever is in the `@` (at sign) will be what is finally rendered
> inside the `{{` , `}}` block.
> This `@` variable is both fully readable and fully writable.
> For example, the source below:
```html
<p>{{ @ = "hello" }}</p>
```
> Will be rendered as:
```html
<p>hello</p>
```
> And as mentioned, this `@` variable can be modified however you want:
```html
<p>{{ @ = concat "hello" " world" }}</p>
```
> This will be rendered as:
```html
<p>hello world</p>
```
#### $
> The `$` (dollar sign) is a variable that is automatically exposed within functional
> body scopes.
> In other words, the `$` (dollar sign) is equal to the first argument a function takes in.
> There is also another alias for the first argument in the function, which is `$0`.
> To access the rest of the arguments, you can use `$1`, `$2`, `$3`, `$4`...
> and so on...
### Functions
> If you are familiar with the shell, functions in GPP will be easy for you.
> They follow the same pattern:
```bash
<name-of-function> <arguments...>
```
> In other words, you start by typing the name of the function,
> and then you give it a list of arguments separated by space (` `).
### Practical Example
> Below is an example where we are mapping a list of names ( a list of strings )
> to `<li>` elements within a `<ul>` tag.
```html
<html>
    <head>
    </head>
    <body>
        {{
            @ = Σ names
            ((
                <ul>
                    <li>{{ @ = $ }}</li>
                </ul>
            ))
        }}
    </body>
</html>
```
> Here, `@` is assigned to the result of the function `Σ` (sigma).
> `names` is a variable that is passed to the first argument of the function,
> the second argument is a `((` `))` (ignored block).
> This ignored block contains some HTML.
> (the preprocessor doesn't really know about HTML, it is just text in the eyes of the interpreter).
> Inside the HTML (or the text) is another `{{` , `}}` templating block.
> In this block `@` is assigned to the first argument of the function passed to the
> `Σ` function.
> In this case, the `((` , `))` block is automatically wrapped by a temporary function
> by the interpreter behind the scenes.
