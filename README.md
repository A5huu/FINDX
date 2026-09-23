# FINDX — Mini Search Engine

FINDX is a **local document search engine** being developed as a Data Structures and Algorithms (DSA) based PBL project.

The goal is to build a small search engine from the ground up so that we understand how real search systems work internally instead of depending completely on ready-made search libraries.

## What FINDX Does

FINDX will allow a user to:

- Add local documents to the search system.
- Extract and clean text from those documents.
- Build an index of the words found in the documents.
- Search for words or combinations of words.
- Rank documents according to their relevance.
- Display useful search results and snippets.
- Store document information and search-related data for later use.

## Basic Idea

Instead of checking every document whenever a user searches, FINDX creates an **inverted index**.

For example:

```text
Document 1 → "machine learning is useful"
Document 2 → "machine learning uses data"
Document 3 → "data science is useful"
```

The index becomes:

```text
machine  → D1 → D2
learning → D1 → D2
useful   → D1 → D3
data     → D2 → D3
science  → D3
```

This makes it much faster to find which documents contain a particular word.

## Main Data Structures

FINDX is designed around DSA concepts:

- **Hash Table** — quickly finds a word in the index.
- **Linked List** — stores the documents associated with a word.
- **Stack** — supports query processing where required.
- **Queue** — supports ordered processing tasks.
- **Binary Search Tree** — can be used for sorted vocabulary/search operations.
- **Priority Queue / Heap** — helps rank search results.
- **Arrays and Strings** — support basic storage and text processing.
- **Sorting and Searching algorithms** — support result organization and lookup.

## System Flow

```text
Documents
    ↓
Text Extraction
    ↓
Text Cleaning
    ↓
Tokenization
    ↓
Stop-word Removal
    ↓
Inverted Index
    ↓
Query Processing
    ↓
Search
    ↓
Relevance Scoring
    ↓
Ranking
    ↓
Search Results
```

## Development Approach

The project will be built in small modules instead of writing the entire search engine at once.

### Current development order

1. Text Extraction
2. Text Cleaning / Tokenization
3. Inverted Index
4. Hash Table
5. Document Linked Lists
6. Search and Query Processing
7. Relevance Scoring
8. Result Ranking
9. Persistence
10. Search History / Statistics
11. User Interface
12. Testing and Optimization

## Current Module

The first development module is the **Text Extraction Module**.

It reads a text file character by character, builds words, converts them to lowercase, removes basic stop words, and produces clean words for the indexing module.

See:

- [`extractor.md`](extractor.md)
- [`PRD.md`](PRD.md)

## Example

Input:

```text
Machine learning is useful for data science.
Machine learning uses Python.
```

Clean output:

```text
machine
learning
useful
data
science
machine
learning
uses
python
```

Later, these words will no longer simply be printed. They will be passed to the indexing system.

## Project Goal

The final goal is to have a working local search engine that demonstrates important DSA concepts through a practical application.

The project should be understandable enough that each major feature can be explained in a viva:

> **What data structure is being used, why it is being used, how it works, and what happens internally when the user performs a search.**
