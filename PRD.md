# FINDX — Product Requirements Document (PRD)

## 1. Project Overview

**Project Name:** FINDX — Mini Search Engine

**Project Type:** Project-Based Learning (PBL)

**Primary Focus:** Data Structures and Algorithms

FINDX is a local document search engine designed to demonstrate how the core ideas behind search engines can be implemented using fundamental data structures and algorithms.

The system will index local documents, process user queries, find matching documents, calculate relevance, rank the results, and present them to the user.

The project will be developed in independent modules so that every part can be tested and understood separately.

---

# 2. Problem Statement

Searching manually through many local documents becomes difficult as the number of documents increases.

A simple approach would be:

```text
User Query
    ↓
Open Document 1
    ↓
Search
    ↓
Open Document 2
    ↓
Search
    ↓
Open Document 3
    ↓
Search
```

This repeatedly scans documents.

FINDX solves this by building an **inverted index** before searching.

```text
Documents
    ↓
Extract Words
    ↓
Build Index
    ↓
User Query
    ↓
Look Up Words
    ↓
Find Matching Documents
    ↓
Rank Results
```

---

# 3. Goals

## Primary Goals

- Build a working local document search engine.
- Demonstrate DSA concepts through a practical project.
- Implement an inverted index.
- Search documents efficiently.
- Support basic query processing.
- Rank search results.
- Keep the system modular and easy to understand.
- Provide enough internal visibility for DSA/PBL demonstration.

## Learning Goals

The project should demonstrate:

- File handling
- Strings
- Arrays
- Hash tables
- Linked lists
- Stacks
- Queues
- Trees
- Priority queues/heaps
- Searching
- Sorting
- Basic indexing
- Relevance scoring
- File/database persistence

---

# 4. Non-Goals

The first version is not intended to be:

- A web search engine.
- A replacement for Google or other internet search engines.
- A full natural-language AI search engine.
- A production-scale distributed search platform.

The focus is on understanding and implementing the underlying concepts.

---

# 5. Target Users

The main user is a person who wants to search a collection of local documents.

The project is also intended for:

- Students
- Developers learning DSA
- PBL evaluators
- Anyone demonstrating how a search engine works internally

---

# 6. High-Level Architecture

```text
                  LOCAL DOCUMENTS
                         │
                         ▼
                ┌─────────────────┐
                │ Text Extraction │
                └────────┬────────┘
                         ▼
                ┌─────────────────┐
                │ Text Cleaning   │
                │ & Tokenization  │
                └────────┬────────┘
                         ▼
                ┌─────────────────┐
                │ Inverted Index  │
                └────────┬────────┘
                         │
             ┌───────────┴───────────┐
             ▼                       ▼
        Hash Table              Document Lists
        Word Lookup             D1 → D2 → D3
             │
             ▼
        Query Processing
             │
             ▼
        Search / Matching
             │
             ▼
       Relevance Scoring
             │
             ▼
       Result Ranking
             │
             ▼
       Search Results
```

---

# 7. Main Components

## 7.1 Document Manager

Responsible for:

- Registering documents.
- Assigning document IDs.
- Storing document names and paths.
- Passing documents to the extractor.

Example:

```text
Document ID: 1
Name: notes.txt
Path: documents/notes.txt
```

---

## 7.2 Text Extraction Module

Responsible for:

- Opening a document.
- Reading its contents.
- Reading characters.
- Detecting word boundaries.
- Building words.
- Passing words to the cleaning/indexing stage.

The first version will primarily support `.txt` files.

Detailed design is documented in `extractor.md`.

---

## 7.3 Text Cleaning and Tokenization

The extracted text will be normalized.

Operations include:

- Lowercase conversion.
- Punctuation removal.
- Word separation.
- Stop-word removal.
- Basic validation of tokens.

Example:

```text
"Machine, Learning is Useful!"
```

becomes:

```text
machine
learning
useful
```

---

# 8. Inverted Index

The inverted index is the central data structure of FINDX.

Instead of:

```text
Document → Words
```

the system stores:

```text
Word → Documents
```

Example:

```text
machine  → D1 → D2
learning → D1 → D2
data     → D2 → D3
```

---

# 9. Hash Table

The hash table provides fast access to a word.

Conceptually:

```text
Hash Table
     │
     ├── "machine" → WordNode
     ├── "data"    → WordNode
     └── "learning"→ WordNode
```

Each WordNode contains the word and a pointer to its document list.

Possible structure:

```c
typedef struct WordNode {
    char word[50];
    struct DocNode *documents;
    struct WordNode *next;
} WordNode;
```

The `next` pointer can support collision handling through chaining.

---

# 10. Document Linked List

Every word points to a linked list of documents containing that word.

Example:

```text
machine
   ↓
D1 → D2 → D4 → NULL
```

A document node can contain:

```c
typedef struct DocNode {
    int doc_id;
    char doc_name[100];
    int frequency;
    struct DocNode *next;
} DocNode;
```

Possible future fields include word positions or other metadata.

---

# 11. Search Process

For a query such as:

```text
machine learning
```

the system will:

```text
Query
  ↓
Tokenize
  ↓
Find "machine"
  ↓
Find "learning"
  ↓
Get document lists
  ↓
Combine/intersect results
  ↓
Calculate relevance
  ↓
Rank
  ↓
Display
```

---

# 12. Query Processing

The project can support basic query operators such as:

```text
machine AND learning
machine OR data
machine NOT hardware
```

The query processor will determine what operation should be performed.

A stack can be used for expression/operator processing where appropriate.

---

# 13. Relevance Scoring

A document that matches a query more strongly should receive a higher score.

The first version can use simple scoring.

Possible factors:

- Number of query terms matched.
- Frequency of query words in the document.
- Number of distinct query terms matched.
- Optional future position/proximity information.

Example:

```text
Query: machine learning

D1:
machine = 4
learning = 3
Score = higher

D2:
machine = 1
learning = 1
Score = lower
```

The exact scoring formula will be finalized during the ranking phase.

---

# 14. Result Ranking

After scoring:

```text
D1 → 8
D2 → 5
D3 → 2
```

the system should return:

```text
1. D1
2. D2
3. D3
```

A priority queue/heap can be used to efficiently manage the highest-scoring results.

Sorting algorithms can also be used where appropriate.

---

# 15. Persistence

A linked list created using `malloc()` exists only in RAM while the program is running.

Pointers should not simply be written to a file because memory addresses are not permanent.

Therefore persistence will store **data**, not raw pointer addresses.

Possible approaches:

### Initial version

Rebuild the index from the original documents whenever the program starts.

### Later version

Save index records in a file:

```text
machine|1|doc1.txt|2
machine|2|doc2.txt|1
learning|1|doc1.txt|2
```

### Future version

Use SQL tables for document and index metadata.

---

# 16. Data Model

A conceptual database structure can contain:

### Documents

```text
doc_id
doc_name
file_path
file_type
```

### Words

```text
word_id
word
```

### Postings

```text
word_id
doc_id
frequency
```

### Search History

```text
search_id
query
timestamp
```

This database layer can be added after the core in-memory DSA implementation works.

---

# 17. Development Phases

## Phase 1 — Text Extraction

Build:

- File opening
- Character reading
- Word detection
- Lowercase conversion
- Basic punctuation handling
- Stop-word removal

Output clean words.

**Current phase.**

---

## Phase 2 — Tokenization and Cleaning

Improve:

- Token validation
- Punctuation rules
- Number handling
- Stop-word list
- Duplicate handling

---

## Phase 3 — Inverted Index

Build:

- WordNode
- Document Node
- Word → Document relationship
- Document frequency tracking

---

## Phase 4 — Hash Table

Implement:

- Hash function
- Buckets
- Collision handling
- Word lookup
- Word insertion

---

## Phase 5 — Search

Implement:

- Single-word search
- Multiple-word search
- AND
- OR
- NOT

---

## Phase 6 — Scoring and Ranking

Implement:

- Term frequency
- Match score
- Priority queue/heap
- Sorted results

---

## Phase 7 — Persistence

Implement:

- Index save/load
- Document metadata
- Optional SQL integration

---

## Phase 8 — Search History and Statistics

Track:

- Previous queries
- Number of searches
- Number of indexed documents
- Number of unique words
- Search time

---

## Phase 9 — User Interface

Possible interface:

```text
+--------------------------------------+
|              FINDX                   |
|                                      |
| Search: [ machine learning       ]   |
|                                      |
| 1. Machine Learning Notes            |
|    ...machine learning...            |
|                                      |
| 2. ML Introduction                   |
|    ...machine learning...            |
+--------------------------------------+
```

---

# 18. Error Handling

The system should handle:

- Missing files.
- Empty files.
- Invalid file paths.
- Very long words.
- Unsupported file types.
- Duplicate documents.
- Empty queries.
- Invalid query operators.

---

# 19. Testing Strategy

Each module should be tested separately.

### Text extraction tests

Test:

- Normal text.
- Empty file.
- Punctuation.
- Uppercase/lowercase.
- Multiple spaces.
- Numbers.
- Stop words.
- File-not-found errors.

### Index tests

Test:

- New word insertion.
- Existing word insertion.
- New document for an existing word.
- Duplicate word in the same document.
- Hash collisions.

### Search tests

Test:

- Existing word.
- Missing word.
- AND query.
- OR query.
- NOT query.
- Empty query.

---

# 20. Success Criteria

The project is considered functional when:

- Documents can be read successfully.
- Text can be cleaned into tokens.
- Words can be inserted into the inverted index.
- Documents can be associated with words.
- Queries can find matching documents.
- Results can be scored and ranked.
- The major DSA components can be demonstrated and explained.

---

# 21. Design Principle

The most important principle of FINDX is:

> **Build the search engine ourselves using understandable data structures before adding advanced libraries or optimizations.**

Every major feature should answer four questions:

1. What problem does it solve?
2. What data structure is used?
3. How does the algorithm work?
4. Why was this approach chosen?

This makes the project useful not only as a search engine, but also as a practical demonstration of DSA.
