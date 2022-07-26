# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'SimpleInterpolator'
copyright = '2022, Naoki Hori'
author = 'Naoki Hori'

# The full version, including alpha/beta/rc tags
release = '0.1'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
        'sphinx.ext.mathjax',
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'alabaster'
html_theme_options = {
    "fixed_sidebar": "true",
    "github_user": "NaokiHori",
    "github_repo": "SimpleInterpolator",
    "github_type": "true",
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

mathjax_path = "https://cdn.jsdelivr.net/npm/mathjax@2/MathJax.js?config=TeX-AMS-MML_HTMLorMML"
mathjax3_config = {
    "TeX": {
        "Macros": {
            "vat": ["{\\left. {#1} \\right|_{#2}}", 2], # value at
            "der": ['{\\frac{\\partial #1}{\\partial #2}}', 2],
            "dder": ['{\\frac{\\delta #1}{\\delta #2}}', 2],
            "pim":  ["i-\\frac{1}{2}"],
            "pic":  ["i             "],
            "pip":  ["i+\\frac{1}{2}"],
            "pjm":  ["j-\\frac{1}{2}"],
            "pjc":  ["j             "],
            "pjp":  ["j+\\frac{1}{2}"],
            "xim":  ["i             "],
            "xic":  ["i+\\frac{1}{2}"],
            "xip":  ["i+1           "],
            "xjm":  ["j-\\frac{1}{2}"],
            "xjc":  ["j             "],
            "xjp":  ["j+\\frac{1}{2}"],
            "yim":  ["i-\\frac{1}{2}"],
            "yic":  ["i             "],
            "yip":  ["i+\\frac{1}{2}"],
            "yjm":  ["j             "],
            "yjc":  ["j+\\frac{1}{2}"],
            "yjp":  ["j+1           "],
            "ux": ['{u_x}'],
            "uy": ['{u_y}'],
        }
    }
}

