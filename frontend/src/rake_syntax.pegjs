rake = origin:origin ":" spacing:nail_spacing { return {origin, "spacing": spacing.reverse()}} / spacing:nail_spacing { return {"origin": 0, "spacing": spacing.reverse()}}
origin = number
nail_spacing =  num:number_not_null fil:"," oth:nail_spacing { oth.push(num); return oth} / num:number_not_null {return [parseInt(num)]}
number = num:digit* { return parseInt(num.join(""))}
number_not_null = num:number
digit = "0" / digit_not_null
digit_not_null = "1" / "2" / "3" / "4" / "5" / "6" / "7" / "8" / "9"
