(TeX-add-style-hook
 "TianyiGu2017Summer"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("beamer" "xcolor=x11names" "compress")))
   (TeX-run-style-hooks
    "latex2e"
    "beamer"
    "beamer10"
    "graphicx"
    "tikz"
    "palatino")))

