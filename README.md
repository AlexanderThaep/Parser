This is my own implementation of a Regular Expression engine in C based off of this [video](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=video&cd=&cad=rja&uact=8&ved=2ahUKEwjg1oia2qKDAxUeQjABHfRZAqsQtwJ6BAgTEAI&url=https%3A%2F%2Fwww.youtube.com%2Fwatch%3Fv%3Du01jb8YN2Lw&usg=AOvVaw204mcjZ0xRkhD6FRxvW-QT&opi=89978449) by **Low Byte Productions**

Regular expressions are used pretty everywhere involving user input since we communicate in languages and text. In its most basic form, it scans through text and finds characters. At its most complex, we have engines built for efficiency with advanced capabilities supporting a wide array of platforms.

However, this implementation seeks to allow for the minimum that any user familiar with RegEx would appreciate. There are few special features and even fewer guarantees of efficiency or function. Yet, it's a little novelty project that I hope to incorporate into a larger Language Parser and, eventually, compiler.

**Current features:**

<ul>
  
  Matches literals (ex. 1, e, r, k)
  
  Matches anything: .
  
  Matching one or more: +

  Matching zero or more: *

  Matching zero or one: ?

  Variable matching: {,}
</ul>

**TODO features:**

<ul>
  
  ~~Literal sets~~

  ~~Caret negation~~
  
  Backtracking
  
  Extra features...
</ul>
