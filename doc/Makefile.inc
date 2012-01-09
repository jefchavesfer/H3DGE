all:: $(TARGET)

compile::
	pdflatex $(TEX_SRC) -o $(TARGET)

bibliography: $(BIBTEX_SRC) $(TEX_SRC)
	pdflatex $(TEX_SRC) -o $(TARGET)
	bibtex $(patsubst %.bib, %, $(BIBTEX_SRC))

$(TARGET): $(TEX_SRC) bibliography
	pdflatex $< -o $@
	pdflatex $< -o $@

clean::
	rm -f *.aux *.log *.nav *.out *.snm *.toc *.bbl *.blg *.pdf *.lot *.lof *~
