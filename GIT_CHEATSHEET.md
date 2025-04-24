# 💻 Git + GitHub Cheat Sheet for Common Projects

This cheat sheet documents common Git and GitHub workflows used in this repo. It applies to both **Windows and Linux** setups using SSH.

---

## 🔧 One-Time Git Repo Setup

```bash
git init
git remote add origin git@github.com:YourUsername/RepoName.git
```

> If using HTTPS, replace with:
> `https://github.com/YourUsername/RepoName.git`

---

## 🗃️ File Tracking Commands

```bash
git status              # Show modified/untracked files
git add .               # Stage all changes
git add <file>          # Stage individual file
git restore <file>      # Discard local file changes
```

---

## 📝 Committing Changes

```bash
git commit -m "Descriptive message here"
```

---

## 📤 Pushing to GitHub

```bash
git push                # Push to origin (must have upstream set)
git push -u origin main # Use for first-time push
```

---

## 📥 Pulling Updates

```bash
git pull                # Standard pull (merge)
git pull --rebase       # Reapply local commits on top of remote
```

---

## 🔁 Branch Management

```bash
git branch                  # List branches
git branch -M main          # Rename current branch to main
git checkout -b <new-branch>  # Create & switch to new branch
git checkout main           # Switch back to main branch
```

---

## 🔐 SSH Setup for GitHub

```bash
ssh-keygen -t ed25519 -C "your_email@example.com"

# Add public key (~/.ssh/id_ed25519.pub) to:
# https://github.com/settings/keys

git remote set-url origin git@github.com:YourUsername/RepoName.git
```

---

## 🛠️ GitHub Actions CI Setup

### File: `.github/workflows/build.yml`

```yaml
name: Build Firmware

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
    - name: Checkout repo
      uses: actions/checkout@v3

    - name: Install ARM toolchain
      run: |
        sudo apt update
        sudo apt install -y cmake ninja-build gcc-arm-none-eabi

    - name: Configure project
      run: cmake -B build -G Ninja

    - name: Build
      run: cmake --build build

    - name: Firmware Size
      run: arm-none-eabi-size build/ProjectName.elf || true #Update with Project Name
```

### Then:

```bash
git add .github/workflows/build.yml
git commit -m "Add CI build workflow"
git push
```

---

## 📋 Helpful Git Diagnostics

```bash
git log --oneline        # Short commit history
git diff                 # See unstaged changes
git diff --cached        # See staged changes
```

---

## ⚠️ Handling Diverged Branches

```bash
git pull --rebase
git push
```

This keeps the commit history clean if your local and remote differ.

---

## 🧪 Git Config (First-Time Setup)

```bash
git config --global user.name "User Name"
git config --global user.email "your@email.com"
```

---

## 📦 Recommended .gitignore Entries

```
build/
*.elf
*.bin
*.hex
*.map
.vscode/
*.launch.json
```

---

## 💬 Maintainer

This repo is maintained by **Fallen Evangelion** / ElectronicsBuilder  
You’re free to fork and adapt this sheet to your embedded projects.