@echo off
docgen hftw_mem.h > docs.tmp
htmlgen docs.tmp title.txt overview.html > docs.html