# Surviving-Maps-CC-Modernish
A more modern implementation of the original Surviving-Maps project. This implementation splits the whole platform into three seperate sub-services dedicated to specific functions: Web, data ingest, and request handling.

This is a multi-project repository containing:

### Surviving-Maps-CC (React front-end)
A re-implmentation of the web portion of Surviving-Maps-CC with React and Vite.

### Surviving-Maps-Ingest
A C++ ingest for transforming ChoGGi's Surviving Mars .csv's into a more streamlined and searchable form. Essentially pre-categorizing and formatting, for quicker and more reliable searchability.
Each .csv is~ 50,900 potential sites of interest, and the intent is to provide as accurate results as possible as fast as possible. This requires some level of pre-processing.

### Surviving-Maps-CPP (Back-end)
The C++ Backend for responding to search queries received via the React front-end.
