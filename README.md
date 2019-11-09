# DAG

Generate DAGs from dependency lists.

## Building

- `mkdir build && cd build && cmake .. && make && ctest ..`

## Usage

- `cat exampledag.txt | ./dag`
- `echo "a>b,b>c" | ./dag`

## Releases

- 1.0.0 - Initial release
- 1.1.0 - Improved graph display
- 1.1.2 - Bugfixes
- 1.1.3 - Swich to cmake