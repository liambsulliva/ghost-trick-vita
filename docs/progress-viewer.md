# Function treemap viewer

GitHub renders README images and repository SVG previews as images. Links inside the SVG cannot receive clicks in those previews. Wrapping the image in a Markdown link only makes the whole image open one URL. The GitHub Markdown API also removes inline SVG shapes and HTML image-map regions, so those cannot provide individual function links in the README.

The README uses a compact static SVG. The standalone viewer embeds a separate SVG as a document, where each green rectangle links directly to its verified C/C++ file on GitHub. The source links include the revision used to build the chart. Gray rectangles have no verified source yet.

The [interactive viewer](https://liambsulliva.github.io/ghost-trick-vita/) uses GitHub Pages. The repository and viewer are public. The README preview links to this page rather than GitHub's SVG file viewer. See [GitHub's custom workflow documentation](https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages).

Only `index.html` and `treemap.svg` are included in the site artifact. They contain chart geometry, function names, addresses, instruction modes, sizes, matching status, source paths and links. They contain no C/C++ source contents, ROM bytes, game art, extracted assets or compiler tools. GitHub still controls access to the linked source files.

To build a local preview from a checkout:

```sh
python3 -I tools/progress_treemap.py \
  --site-dir build/progress-site \
  --source-url "https://github.com/liambsulliva/ghost-trick-vita/blob/$(git rev-parse HEAD)/"
open -a Safari build/progress-site/index.html
```

The build uses the symbol configuration, verification ledger and source files already in the repository. It needs no ROM, compiler or extracted data.

Pages uses **GitHub Actions** as the publishing source. Run the **Progress treemap** workflow to update the viewer after pushing a verified checkpoint. The workflow runs only when manually dispatched and uploads only `build/progress-site`. Its source links use that workflow's commit, so the chart remains consistent with the source it opens.
