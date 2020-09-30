# GPP
> General PreProcessor  
> _not just for HTML, it can be used anywhere_

```html
<!--
    {{ }} = ** Compute Block ** 
          - A block that is interpreted as templating logic

    (@ @) = ** Virtual File Block **
          - Anything in here is just treated as if it was a completely new file.
            This block is also used to tell the pre-processor that something
            should be evaluated "lazy".

    ( ) = ** List **
          - A list of elements, separated by comma.
          - Aliases: [ ]

    map = ** Built-in function **
          - It does what you expect, just like the map function in for example
            Javascript, it takes in an iterable as the first argument and a
            yield / function as the second argument. 

    cat = ** Built-in function **
          - This function simply reads an infinite list of files, concatenates
            the contents and dumps it out as it is.
            The contents of the files are just treated as text and not evaluated.
            This function takes in an infinite list of strings as an argument.

    cate = ** Built-in function **
          - Just like `cat` but it also evaluates the content of the files
            before concatenating them and dumping it out.

    floor = ** Built-in function **
          - Floors a number. For example, the input 60.6 would return 60.

    key = ** Built-in function **
          - Extract a key by index from an object.
            The signature is:
            key (object index)

    value = ** Built-in function **
          - Extract a value by index from an object.
            The signature is:
            value (object index)

    get = ** Built-in function **
          - Extract a value by index or key from an object or list.
            The signature is:
            value ((object || list) (index || key))

    $ = ** Positional parameter constant **
          - This is heavily inspired by the shell, it allows you to access
            the positional arguments within a function body.
            For example, to access the first argument you would use `$0`,
            to access the second one you would use `$1`, and then `$2`...
            and so on and so forth. 
!-->
<html>
    <body>
        <h1>{{ "Welcome" }}</h1>
        <h2>The basics</h2>
        <section>
            <h3>Virtual file blocks and compute blocks</h3>
            <span>
                (@
                    Here is some text that will be completely ignored.   
                    This: <b>{{ "Is not ignored however" }}</b>.
                @)
            </span>

            <h3>Mapping a list to HTML</h3>
            <ul>
               {{ map (["apple", "banana", "pear","orange"], (@<li>{{ $0 }}</li>@)) }}
            </ul>

            <h3>Dumping out file contents</h3>
            <pre>{{ cat ("src/main.c") }}</pre>

            <h3>Below will be evaluated</h3>
            <p>{{ cate ("examples/hello.html") }}</p>
        </section>
        <h2>Integrating other languages</h2>
        <section>
            <h3>I really want this list to be rendered using Python!</h3>
            <ul>
            (@#!/usr/bin/python
            for name in ["john", "sarah", "anna"]:
                x = """(@
                    <li>
                        {}     
                    </li>
                @)""".format(name)
                print(x)
            @)
            </ul>
            <h3>I do want this list to be rendered in Node.js though</h3>
            <ul>
            (@#!node
            console.log(["john", "sarah", "anna"]
              .map(name => (@ "<li>" + name + "</li>" @)).join('\n'));
            @)
            </ul>
        </section>
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
#### (@ , @)
> The `(@` and the `@)` tokens are used to indicate the start `(@` and the end `@)`
> of a virtual file block.
> A virtual file block is something that is just treated as text by the preprocessor.
> In other words, nothing within these tokens will be interpreted.
> The entire document is by default wrapped in a virtual file block.  
> **There is one exception** to the `(@ @)` (virtual file block) however,
> if you specify a path to an interpreter, the rest of the content will be
> interpreted. _(There is an example further down in this document)_
#### If you still do not understand
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
<p>(@ "hello" @)</p>
```
> Will be rendered as:
```html
<p> "hello" </p>
```
#### Arrow functions
> You can also define functions to render data,  
> Here is an example of creating a function to render a `<ul>` list:
```jsx
{{
    itemList = (items) => map (items, (@
        <li>{{ $0 }}</li>
    @))
}}

{{  itemList (["hello", "world"]) }}
```
> This will render as:
```html
<ul>
    <li>hello</li>
    <li>world</li>
</ul>
```
### Extending / Inheritance
> You can make templates inherit / extend other templates.  
> Here is how a parent template could look like: (`parent.hml`)
```html
<html>
    <head>
        <title>(@#%block title @)</title>
    </head>
    <body>
        (@#%block body @)
    </body>
</html>
```
> Notise the `%` at the start of the comment.  
> And then you can write a child template that inherits from this one  
> Like this: (`child.html`)
```html
#%extends "parent.html"

(@#%block title
    My Title
@)

(@#%block body
    Welcome to my website
@)
```
> Also, notice here the `%` at the start of each comment.  
> If you run `gpp` over `child.html` now, the output will be:
```html
<html>
    <head>
        <title>My Title</title>
    </head>
    <body>Welcome to my website</body>
</html>
```

## Loading data
> What is a templating engine without any data to work with?  
> To access data in your templates, simply put a `context.json` file
> within the root of your project.  
> Example (`context.json`):
```json
{
    "title": "My Title",
    "favouriteFruits": ["apple", "pear", "banana"]
}
```
> Now you can access these variables in your templates,  
> Example:
```html
<html>
    <title>{{ title }}</title>
    <body>
        {{ map (favouriteFruits, (@<li>{{ $0 }}</li>@)) }}
    </body>
</html>
```

## Any interpreter you want
> If you do not want to use the default language,
> you can use any language you want to interpret the templates.
> Here is an example where we are rendering a `<ul>` list using `Python`.
```html
<html>
    <body>
        <ul>
(@#!/usr/bin/python
for i in range(100):
    x = """(@
        <li>
            {}     
        </li>
    @)""".format(i)
    print(x)
 @)
        </ul>
    </body>
</html>
```
> To make this work, we simply put a comment inside our `(@ @)` block.
> This comment should start with a `#!` and then the path to the
> program that should be interpreting the rest of the content inside of the
> `(@ @)` block.

## Installation
> Ready to use this piece of software?
### 1. Clone it (or download it)
> Go and get the source code, if you are reading this on Github,  
> then just copy the clone URL and clone it down.
```
git clone <url> --recurse-submodules
```
> Do NOT forget the `--recurse-submodules` flag. 
### 2. Compile it
> Make sure you have a compiler on your system, preferably `gcc`.
> You can read more about `gcc` here: [https://gcc.gnu.org/](https://gcc.gnu.org/)  
> You also need to make sure you have `make` on your system.  
> You can read more about `make` here: [https://www.gnu.org/software/make/](https://www.gnu.org/software/make/).  
> **Alright!** time to compile this thing.
>
> **Go to the root directory** - Open up a shell and go to the root directory of this project:
```bash
cd gpp/
```
> **Execute Make** - Run the make command
```bash
make
```
> **!!!Abrakadabra!!!**
### 3. Run it
> You are now ready to use this,
> Here is how you do that:
```bash
./a.out index.html
``` 
> Yes... I know `a.out` is not really a production ready name for a binary.  
> But this software is still in the _work in progress_ stage.  
> Anyways, so you execute the `a.out` file and you give it a filename as
> the first argument and then it will just print out the transpiled / compiled
> content to `stdout`.

## Plans
> Here are some plans / thoughts.  
> If you feel like you could help implement some of them, feel free
> to make a pull-request.   
- [ ] Write better documentation
- [ ] Implement a step-by-step debugger
- [ ] Create a shared library to allow the use of this in other languages
- [ ] Implement bindings for NodeJS 
- [ ] Implement bindings for Python  
> Now, these are not tasks / issues.  
> This is just some ideas and I am just thinking out loud.  
> Bugs, Todos, and tasks will be seen in Github's issues section.

## FAQ
```
If I define a variable in my Python code,
how can I later access it in my NodeJS block
below?
```
> That is unfortunately not possible.  
> If you want to share data between languages, you need to use the built-in
> templating language to unserialize the data into your code blocks.
```
Isn't it unsafe to call the binary specified in the "!#" comment?
```
> Well, just don't put an unsafe binary there?  
> This couldn't care less about what logic you write or which
> binaries you use to interpret the code blocks.
> This is just a templating language.
```
Does it work on Windows?
```
> I dont know, I haven't tried it there.  
> But I assume it would be tricky to get it to work there,
> since this software was developed on a Linux machine and the source code
> has dependencies on C implementations on Linux... you know header files
> and stuff.
```
How do I send data into the template context?
```
> Put a `context.json` file in the root of your project,
> you can later access all data in this file inside your templates. 

---
**<div style="width: 100%; text-align: center;" align="center">:coffee: Not Made with Coffee and Love in San Francisco :coffee:</div>**
