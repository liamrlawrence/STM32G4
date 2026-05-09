.PHONY: all clean docs


docs:
	doxygen Doxyfile
	sphinx-build -a -E -b html docs docs/_build
	@echo "Documentation generated at: docs/_build/index.html"

clean:
	rm -rf docs/doxygen docs/_build
	@echo "Cleaned."

