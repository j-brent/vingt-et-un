#!/usr/bin/env node
/**
 * generate-theme.js
 * Generates Theme.qml and theme.css from tokens.json
 *
 * Usage: node scripts/generate-theme.js
 */

const fs = require('fs');
const path = require('path');

const TOKENS_PATH = path.join(__dirname, '..', 'styles', 'tokens.json');
const QML_OUTPUT_PATH = path.join(__dirname, '..', 'app', 'qml', 'Theme.qml');
const CSS_OUTPUT_PATH = path.join(__dirname, '..', 'styles', 'theme.css');

function camelToKebab(str) {
    return str.replace(/([a-z])([A-Z])/g, '$1-$2').toLowerCase();
}

function generateQml(tokens) {
    const lines = [
        '// Auto-generated from styles/tokens.json - do not edit directly',
        '// Run: node scripts/generate-theme.js',
        'pragma Singleton',
        'import QtQuick',
        '',
        'QtObject {',
        '    id: theme',
        '',
        '    // Base unit (1em equivalent) - 16px is standard browser default',
        '    readonly property real em: 16',
        '',
        '    // Colors',
    ];

    for (const [key, value] of Object.entries(tokens.colors)) {
        lines.push(`    readonly property color ${key}: "${value}"`);
    }

    lines.push('');
    lines.push('    // Sizes (em-based)');

    for (const [key, value] of Object.entries(tokens.sizing)) {
        if (key.endsWith('Em')) {
            const propName = key.replace(/Em$/, '');
            lines.push(`    readonly property real ${propName}: em * ${value}`);
        } else {
            lines.push(`    readonly property real ${key}: ${value}`);
        }
    }

    lines.push('');
    lines.push('    // Fonts');

    for (const [key, value] of Object.entries(tokens.fonts)) {
        lines.push(`    readonly property string ${key}: "${value}"`);
    }

    lines.push('}');
    lines.push('');

    return lines.join('\n');
}

function generateCss(tokens) {
    const lines = [
        '/* Auto-generated from styles/tokens.json - do not edit directly */',
        '/* Run: node scripts/generate-theme.js */',
        '',
        ':root {',
        '    /* Colors */',
    ];

    for (const [key, value] of Object.entries(tokens.colors)) {
        const cssVar = `--color-${camelToKebab(key)}`;
        lines.push(`    ${cssVar}: ${value};`);
    }

    lines.push('');
    lines.push('    /* Sizes (em-based) */');

    for (const [key, value] of Object.entries(tokens.sizing)) {
        if (key.endsWith('Em')) {
            const propName = camelToKebab(key.replace(/Em$/, ''));
            lines.push(`    --${propName}: ${value}em;`);
        } else {
            const propName = camelToKebab(key);
            lines.push(`    --${propName}: ${value};`);
        }
    }

    lines.push('');
    lines.push('    /* Fonts */');

    for (const [key, value] of Object.entries(tokens.fonts)) {
        const cssVar = `--font-${camelToKebab(key)}`;
        lines.push(`    ${cssVar}: ${value};`);
    }

    lines.push('}');
    lines.push('');

    return lines.join('\n');
}

function main() {
    console.log('Reading tokens from:', TOKENS_PATH);

    const tokensRaw = fs.readFileSync(TOKENS_PATH, 'utf8');
    const tokens = JSON.parse(tokensRaw);

    const qmlContent = generateQml(tokens);
    const cssContent = generateCss(tokens);

    fs.writeFileSync(QML_OUTPUT_PATH, qmlContent);
    console.log('Generated:', QML_OUTPUT_PATH);

    fs.writeFileSync(CSS_OUTPUT_PATH, cssContent);
    console.log('Generated:', CSS_OUTPUT_PATH);

    console.log('Done!');
}

main();
