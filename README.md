# The Circa Library Set

Circa is a library set for C aiming to make the language easier to use for
common tasks that would benefit from dynamically sized generic datatypes.

Documentation and explanations can be found [here](doc/README.md).

A table of all of the implemented types can be found below.

| Type      | Description                                                       |
| --------- | ----------------------------------------------------------------- |
| Dict(T)   | A Robin Hood hash table of `char* -> T` using open addressing.    |
| Map(K, V) | Exactly like `Dict(T)` but `K -> V` instead.                      |
| Seq(T)    | A dynamically sized array of `T`. Analogous to C++ `std::vector`. |
| Txt       | A dynamically sized ASCII string. Analogous to C++ `std::string`. |
| Slice     | A generic range type consisting of two `size_t` values.           |
