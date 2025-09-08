# AirLang Research Paper

## AirLang: A Domain-Specific Language for Aviation Flight Planning and Operations

**Author:** Tisha Patel  
**Publication Date:** September 2025  
**DOI:** [10.13140/RG.2.2.25987.57120](https://doi.org/10.13140/RG.2.2.25987.57120)  
**License:** CC BY 4.0  

### Links
- **ResearchGate:** https://www.researchgate.net/publication/395303012_AirLang_A_Domain-Specific_Language_for_Aviation_Flight_Planning_and_Operations
- **DOI Link:** https://doi.org/10.13140/RG.2.2.25987.57120
- **PDF:** [AirLang_Paper.pdf](./AirLang_Paper.pdf) 
### Abstract
This paper presents AirLang, a domain-specific 
language (DSL) designed for aviation flight planning and dispatch 
operations. The language offers aviation-focused syntax with 
native support for aircraft identifiers, flight numbers, coordinate 
pairs, and specialized navigation functions including great circle 
distance calculations and wind component analysis. AirLang 
employs a hierarchical block-structured format that directly 
mirrors standard aviation documentation workflows and 
operational procedures.
The implementation features a complete seven-stage compiler pipeline written in C, incorporating lexical analysis via finite state machines, recursive descent parsing with formal grammar rules, semantic validation with embedded aviation safety constraints, and bytecode generation for a custom virtual machine with specialized aviation instruction types.
The compiler successfully validates aviation identifiers, performs 
automated weight and balance calculations with safety limit 
checking, processes meteorological data, and generates executable 
flight dispatch reports. This work demonstrates practical 
applicability to aviation workflow modeling by demonstrating 
how domain-specific language principles can be applied to safety
critical technical domains, while showing how domain expertise 
can effectively inform programming language design decisions 
and validates the feasibility of creating safety-critical specialized 
languages for technical domains.

### Citation
```
Patel, T. (2025). AirLang: A Domain-Specific Language for Aviation Flight Planning and Operations. 
DOI: 10.13140/RG.2.2.25987.57120
```

### BibTeX
```bibtex
@unpublished{patel2025airlang,
  title={AirLang: A Domain-Specific Language for Aviation Flight Planning and Operations},
  author={Patel, Tisha},
  year={2025},
  month={September},
  note={Preprint},
  doi={10.13140/RG.2.2.25987.57120},
  url={https://doi.org/10.13140/RG.2.2.25987.57120}
}
```