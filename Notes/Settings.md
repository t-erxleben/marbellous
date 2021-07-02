## Rake syntax ##

`3:1,2,3`.results.in.this.rake:.`X...X..X.X...X..X.X.XO.X...X.X..X...X.X..X...X`

```BNF
rake => [origin ":"] nail-spacing
origin => number
nail-spacing => [number-not-null ","] number-not-null
number => digit + 
number-not-null => digit * digit-not-null
digit => 0 | digit-not-null
digit-not-null => 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
```

* **nail-spacing:** describe the space between last and next nail in ticks
  when the and of the spacing list is reached we start from the begin.
* **origin:** determines the position where the handle is placed at the rake
	its the ticks to the right from the symmetry nail 
