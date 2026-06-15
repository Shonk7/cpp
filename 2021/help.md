## Commands for compiling 
```
 g++ -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined substring.cpp -o substring
```

## if using Cin Cout
```c
int main() {
  cin.tie(nullptr);
  cin.sync_with_stdio(false);
}
```
don't use endl -> use `\n`

