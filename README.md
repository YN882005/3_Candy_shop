## Name: Youssef Nasser Farouk Elsayed Mohamed Kandiel
# Project(3): Candy Shop

# How to Build such project:
Inventory & basket: Store stock in Candy_t shelf structs and active orders in Line_t basket entries.  

Array management: Search existing lines during addToBasket() to consolidate duplicate items. When removing items in removeFromBasket(), shift higher array elements down to prevent holes.  

Change calculation: Use greedy integer division in giveChange() across 500, 200, 100, 50, and 25 piastre coin denominations.
# Answer to why question: 
* All available coin denominations (500, 200, 100, 50, and 25 piastres) are multiples of 25 piastres. Consequently, any linear combination of these coins will yield a value that is strictly divisible by 25. Because 137 piastres is not divisible by 25 (137 % 25 = 12), no integer combination of the available coins can equal 137 piastres.

# The Fix:
* The program must check whether change % 25 == 0 prior to giving coins. If change % 25 != 0, it should reject the payment or decline the transaction.
