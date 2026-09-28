# TestWA — Wwise Audio QA Training

Training project for practical Technical Audio QA work with **Unreal Engine 5.8** and **Wwise 2025.1.11.9262**.

## What is in this repository

- Unreal project configuration, C++ source, Blueprints, and content
- Wwise project authoring data and SoundBank definitions
- Audio QA / CI-related project code

## What is intentionally not in Git

- The installed `Plugins/Wwise` integration and its SDK binaries
- Unreal build products, caches, logs, and `Saved/`
- Wwise cache and generated SoundBanks
- `TestWA_WwiseProject/Originals/` raw audio files

The exclusions keep the repository practical to clone and within GitHub limits. If the raw audio must be versioned later, use Git LFS with agreed storage limits.

## Local requirements

- Unreal Engine 5.8
- Wwise 2025.1.11.9262 and the matching Unreal integration

Install the Wwise integration into `Plugins/Wwise`, then generate SoundBanks locally before running the project.
