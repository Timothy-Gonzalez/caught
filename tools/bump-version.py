from os import path
import re
import subprocess

try:
    subprocess.check_output(['git', '--version'])
except (subprocess.CalledProcessError, FileNotFoundError):
    print("git not installed!")
    exit(1)
try:
    subprocess.check_output(['git', 'diff', "--cached", "--exit-code"])
except (subprocess.CalledProcessError):
    print("Please commit your changes before bumping version!")
    exit(1)

CAUGHT_ENTRY_H = path.join("src", "caught.h")

with open(CAUGHT_ENTRY_H, 'r') as file:
    content = file.read()

version_pattern = re.compile(r"#define CAUGHT_VERSION_(MAJOR|MINOR|PATCH) (.+)")

version_pattern_matches = version_pattern.findall(content)
current_version = {key: int(value) for key, value in version_pattern_matches}

def format_version(version):
    return f"v{version['MAJOR']}.{version['MINOR']}.{version['PATCH']}"

current_version_str = format_version(current_version)

print(f"Current version: {current_version_str}")
print("1: Bump patch\n2: Bump minor\n3: Bump major")
bump_select = input("> ")

bump_function = {
    '1': lambda v: v.update({"PATCH": v["PATCH"] + 1}),
    '2': lambda v: v.update({"PATCH": 0, "MINOR": v["MINOR"] + 1}),
    '3': lambda v: v.update({"PATCH": 0, "MINOR": 0, "MAJOR": v["MAJOR"] + 1}),
}.get(bump_select)

if not bump_function:
    raise RuntimeError(f"Invalid selection: {bump_select}")

new_version = current_version.copy()
bump_function(new_version)

new_version_str = format_version(new_version)
print(f"Bump to: {new_version_str}")

new_content = version_pattern.sub(
    lambda match: f"#define CAUGHT_VERSION_{match.group(1)} {new_version[match.group(1)]}",
    content
)

version_str_pattern = re.compile(r'#define CAUGHT_VERSION_STRING "(.+)"')
new_content = version_str_pattern.sub(
    lambda _: f'#define CAUGHT_VERSION_STRING "{new_version_str}"',
    new_content
)

with open(CAUGHT_ENTRY_H, 'w') as file:
    file.write(new_content)

subprocess.call(["git", "add", CAUGHT_ENTRY_H])
subprocess.call(["git", "commit", CAUGHT_ENTRY_H, "-m", f"Bump version to {new_version_str}"])
subprocess.call(["git", "tag", new_version_str, "-m", new_version_str])
