# Contributing to Smart AC Controller

Thank you for thinking about contributing. This project is meant to stay
beginner-friendly, affordable, and easy to build. If you have an improvement
that fits those goals, it is very welcome.


---


## Ways to Contribute

Reporting a bug
    Open an issue on GitHub and describe what went wrong. Include:
    - What you expected to happen
    - What actually happened
    - Which version of the code you are using
    - What hardware you have (NodeMCU version, relay brand, etc.)
    - Paste any Serial Monitor output that looks relevant

Suggesting a feature
    Open an issue with the label "enhancement". Describe what you want and why
    it would be useful. If it fits the project goals, it will likely get added.

Fixing a bug
    Fork the repo, make your change on a new branch, and open a pull request.
    Please describe what the bug was and how your fix addresses it.

Adding documentation
    Documentation improvements are always welcome. If something in the README
    or wiring guide confused you, fixing that confusion helps everyone who comes
    after you.

Translating
    If you want to translate the README or other docs into another language,
    create a file like README-hindi.md or README-bengali.md and open a PR.


---


## Code Style Guidelines

These are not strict rules but following them makes reviewing easier.

- Use clear variable names that describe what the variable does
- Add a comment above any line of code that is not immediately obvious
- Keep timing values in config.h, not hardcoded in the .ino
- Do not add features that require expensive additional hardware
- Test on actual hardware before submitting (or clearly note that it is untested)
- Keep things as simple as possible for someone doing this as their first project


---


## What Does Not Fit This Project

- Cloud dependencies (this project works entirely on local Wi-Fi by design)
- Features that require modules costing more than 5 USD each
- Anything that makes the wiring significantly more complicated
- Replacing the ESP8266 with a different platform (that is a separate project)


---


## Pull Request Process

1. Fork the repository
2. Create a branch with a descriptive name: git checkout -b fix-dht-read-timing
3. Make your changes
4. Test on hardware if possible
5. Update the relevant documentation files if your change affects behaviour
6. Add an entry to CHANGELOG.md under an Unreleased section
7. Open a pull request with a clear description of what you changed and why


---


## First Time with Git and GitHub?

That is totally fine. Here is the short version:

1. Create a free account at github.com
2. Go to this project's page and click the Fork button in the top right
3. On your fork, make your changes using the GitHub web editor for small changes
4. Click "Commit changes" and then open a Pull Request from your fork back to the original

For larger changes, you will want to learn git clone and work locally. There are many
free tutorials - "Git and GitHub for beginners" on YouTube is a good start.
