# NoSQLplusplus

NoSQL implementation in C++.

# Name and role

- **Rohit Lunavara (rll2181)**
- **Conor Sweeney (cjs2201)**
- **Ziwei Zhu (zz2556)**

We will start from three basic data structures and implement them using the latest C++20 features.

Each of us can pick one of the following : 

- B tree family.
- R tree family.
- Hash table with collision handling.

# Aim

Creating a Key-Value store like Redis in C++.

# Initial work

Do literature review to find some reference about NoSQL database implementation.

Design API and classes that we would use. For example, we would need **get**, **set**, **add**, **delete**, **find**, etc.

Because we want to support different data structures and different data type, we should separate the interface and the implementation and consider using generic programming.

# Testing

We should use some simple data to test each function.

We can test our program against other standard implementations such as Redis.

# Measurable aspects

Some of the measurable aspects of the project are performance changes in basic operations such as read, write, update, delete due to :

1. Underlying data-structures used to implement the key-value store such as std::unordered_map, std::map and some non STL hash table implementations (HashDB from Kyoto Cabinet which uses BSTs).
2. Collision resolution techniques such as Separate Chaining and Open Addressing (Quadratic Probing, Hopscotch Hashing, etc).

# Plan for releases

1. R0.8

    Implementing three basic data structures for database using C++20.

    Implementing different hash collision techniques to find out the best combination for our implementation.

    No concurrency support.

2. R1.0

    Optimizing for in-memory operations using memory alignment.

    Extending the API to include common operations of NoSQL databases.

    Concurrency support for read.

    Find some fancy applications to show our performance (astronomy, music, image, web)

3. R1.2

    Optimizing for a certain type of secondary memory. (SSDs, HDDs)

    Concurrency support for all operations.

    Supporting other data structures such as list, set, etc.

# References

[https://en.wikipedia.org/wiki/NoSQL](https://en.wikipedia.org/wiki/NoSQL)

[https://en.wikipedia.org/wiki/R-tree](https://en.wikipedia.org/wiki/R-tree)

[https://en.wikipedia.org/wiki/Redis](https://en.wikipedia.org/wiki/Redis)

[https://en.wikipedia.org/wiki/Open_addressing](https://en.wikipedia.org/wiki/Open_addressing)

[https://en.wikipedia.org/wiki/Concurrent_hash_table](https://en.wikipedia.org/wiki/Concurrent_hash_table)

[http://www.cs.cmu.edu/~ab/15-121N11/lectures/lecture16.pdf](http://www.cs.cmu.edu/~ab/15-121N11/lectures/lecture16.pdf)

[http://codecapsule.com/2012/11/07/ikvs-implementing-a-key-value-store-table-of-contents/](http://codecapsule.com/2012/11/07/ikvs-implementing-a-key-value-store-table-of-contents/)
