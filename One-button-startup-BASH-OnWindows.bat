
del *.aux
del *.bbl
del *.blg
del *.log
del *.fls
del *.fdb_latexmk
del *.synctex*
del *.out
del *.spl


pdflatex main.tex
bibtex main
pdflatex main.tex
pdflatex main.tex

del *.aux
del *.bbl
del *.blg
del *.log
del *.fls
del *.fdb_latexmk
del *.synctex*
del *.out
del *.spl
