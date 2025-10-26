# Contributing to Wi-Fi Security Monitor

First off, thank you for considering contributing to Wi-Fi Security Monitor! It's people like you that make this tool better for the cybersecurity education community.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Suggesting Enhancements](#suggesting-enhancements)
  - [Pull Requests](#pull-requests)
- [Style Guidelines](#style-guidelines)
  - [Code Style](#code-style)
  - [Commit Messages](#commit-messages)
- [Ethical Guidelines](#ethical-guidelines)

## Code of Conduct

This project and everyone participating in it is governed by our [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to the project maintainers.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When you create a bug report, include as many details as possible:

* **Use a clear and descriptive title**
* **Describe the exact steps to reproduce the problem**
* **Provide specific examples** to demonstrate the steps
* **Describe the behavior you observed** and what you expected to see
* **Include details about your configuration:**
  - ESP32 board model
  - Arduino IDE version
  - ESP32 board package version
  - Operating system

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, include:

* **Use a clear and descriptive title**
* **Provide a detailed description** of the suggested enhancement
* **Explain why this enhancement would be useful**
* **List any similar features** in other tools if applicable

### Pull Requests

1. **Fork the repository** and create your branch from `main`
2. **Make your changes** following our code style guidelines
3. **Test your changes** thoroughly on actual ESP32 hardware
4. **Update documentation** if you're changing functionality
5. **Ensure your code follows** the existing style
6. **Write clear commit messages** following our guidelines
7. **Open a pull request** with a clear description of your changes

## Style Guidelines

### Code Style

* **Indentation:** Use 2 spaces (not tabs)
* **Line length:** Keep lines under 100 characters when possible
* **Comments:** 
  - Use Doxygen-style comments for functions
  - Add inline comments for complex logic
  - Keep comments clear and professional
* **Variable naming:**
  - Use camelCase for variables and functions
  - Use UPPER_CASE for constants and macros
  - Use descriptive names (avoid single letters except for loops)
* **Function documentation:**
  ```cpp
  /**
   * @brief Brief description of the function
   * 
   * @param paramName Description of parameter
   * @return Description of return value
   */
  ```

### Example Code Style

```cpp
/**
 * @brief Calculate signal strength classification
 * 
 * @param rssi Signal strength in dBm
 * @return String classification ("Excellent", "Good", "Fair", "Poor")
 */
String classifySignalStrength(int rssi) {
  if (rssi > -50) {
    return "Excellent";
  } else if (rssi > -60) {
    return "Good";
  } else if (rssi > -70) {
    return "Fair";
  } else {
    return "Poor";
  }
}
```

### Commit Messages

* **Use the present tense** ("Add feature" not "Added feature")
* **Use the imperative mood** ("Move cursor to..." not "Moves cursor to...")
* **Limit the first line to 72 characters**
* **Reference issues and pull requests** when applicable

Good commit messages:
```
Add MAC address whitelisting feature

- Implement whitelist data structure
- Add commands to add/remove MAC addresses
- Skip whitelisted devices during attack
- Update documentation

Fixes #123
```

## Ethical Guidelines

**CRITICAL:** All contributions must adhere to strict ethical guidelines:

1. **No offensive capabilities enhancement** - Do not contribute code that makes attacks more effective, stealthy, or difficult to detect
2. **Educational focus** - All features should have clear educational or defensive value
3. **Legal compliance** - Ensure your contributions don't facilitate illegal activity
4. **Documentation** - Include warnings about legal/ethical use in any new features
5. **Responsible disclosure** - If you discover vulnerabilities, report them responsibly

### What We Will NOT Accept

* Features that bypass detection mechanisms
* Automated attack capabilities without user interaction
* Integration with tools designed for malicious purposes
* Removal or weakening of legal/ethical warnings
* Obfuscation techniques to hide malicious activity

### What We Encourage

* Improved educational documentation
* Better user interface and usability
* Detection and defensive capabilities
* Code quality improvements
* Hardware compatibility enhancements
* Legitimate security research features

## Testing

Before submitting a pull request:

1. **Test on hardware** - Verify your changes work on an actual ESP32
2. **Check edge cases** - Test with no networks, many networks, weak signals
3. **Verify serial output** - Ensure output is clear and professional
4. **Test stop functionality** - Verify that attacks can be stopped cleanly
5. **Check memory usage** - Ensure changes don't cause memory issues

## Documentation

When adding new features:

1. **Update README.md** if the feature affects usage
2. **Add code comments** for complex logic
3. **Update function documentation** with Doxygen comments
4. **Include usage examples** when applicable

## Questions?

Feel free to open an issue with your question. We're here to help!

## Recognition

Contributors will be recognized in the project. Thank you for helping make Wi-Fi Security Monitor better!

---

**Remember:** This project is for educational purposes only. All contributions should support this mission responsibly.
