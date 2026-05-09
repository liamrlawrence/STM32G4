import os
import subprocess

# -- Project information -----------------------------------------------------
project = "STM32G4"
author = "Your Name"
release = "1.0"

# -- General configuration ---------------------------------------------------
extensions = [
    "breathe",
    "sphinx.ext.autodoc",
    "sphinx.ext.viewcode",
]

templates_path = ["_templates"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

# -- HTML output -------------------------------------------------------------
html_theme = "sphinx_rtd_theme"   # pip install sphinx-rtd-theme
html_static_path = ["_static"]

# -- Breathe configuration ---------------------------------------------------
# Points Breathe at the Doxygen XML output directory
breathe_projects = {
    "STM32G4": "doxygen/xml",
}
breathe_default_project = "STM32G4"
breathe_default_members = ("members", "undoc-members")

