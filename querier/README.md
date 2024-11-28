# Bugs 

- because I don't clear andSequence properly I cannot process multiple and's 
- there is a memory leak everytime there is an `and` in the query because in my score function, I don't free my andSequence counters and this causes issues. When I try to free andSequence after I have my final answer in my orSequence, it causes seg faults when I query. So I thought the issue was that my andSequence was somehow still connected in memory to my result counters (orSequence) and would not be able to output anything. But this is not the case so I left that code commented out. I tried hard to find a way to free my andSequence but I cannot so it will leak. 

# Functionality 
- my querier prints sets of the documents in decreasing order by score and supports 'and' and 'or' operators with precedence and 
- it does not work with multiple and operations in a row if all of the words exist in the query because I don't free the andSequence and reset it properly
