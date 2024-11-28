# Crawler - Tiny Search Engine

## Overview
The Crawler is a key component of the Tiny Search Engine (TSE). It is responsible for crawling a website and retrieving web pages starting from a specified seed URL. It explores web pages up to a given depth and stores these pages locally.

## Implementation
* The Crawler is implemented in C.
* It utilizes data structures like hashtable and bag from the `libcs50` library.
* URLs are normalized and checked for internal validity.
* Crawling respects a specified maximum depth.
* Webpages are saved locally with a unique document ID.

## Usage
To run the crawler: ./crawler seedURL pageDirectory maxDepth

- `seedURL` is the initial web page from which crawling begins.
- `pageDirectory` is the directory where the webpages are saved.
- `maxDepth` is the maximum crawl depth (an integer between 0 and 10).

## Assumptions
* The pageDirectory exists and is writable.
* The seedURL is an internal URL and is valid.

## Compilation
To compile, simply run `make all` in the crawler directory and `make` in the `common` directory. Or simply `make` in the overall directory.

## Testing
The crawler is thoroughly tested with various seed URLs and depths. These tests can be run with `make test`.

## Known Issues
There is a chance that it does not work some typs of duplicate URLs 