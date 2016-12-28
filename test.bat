@echo off
docgen lal.c > docs.tmp
htmlgen docs.tmp title.txt overview.html > docs.html
