# Creating Claude Code Agents

How to define effective custom subagents for Claude Code.

## File Structure

Agents are Markdown files with YAML frontmatter stored in:

| Location | Scope | Priority |
|----------|-------|----------|
| `--agents` CLI flag | Current session only | 1 (highest) |
| `.claude/agents/` | Current project (check into git) | 2 |
| `~/.claude/agents/` | All your projects | 3 |
| Plugin `agents/` directory | Where plugin is enabled | 4 (lowest) |

When names conflict, higher-priority location wins.

## Frontmatter Fields

```yaml
---
name: my-agent              # Required. Lowercase letters and hyphens only.
description: ...            # Required. Claude uses this to decide when to delegate.
tools: Read, Grep, Glob     # Optional. Inherits ALL tools if omitted.
disallowedTools: Write      # Optional. Removed from inherited/specified list.
model: inherit              # Optional. sonnet | opus | haiku | inherit. Default: inherit.
permissionMode: default     # Optional. default | acceptEdits | dontAsk | bypassPermissions | plan.
skills:                     # Optional. Skill names to preload into agent context.
  - skill-name-1
  - skill-name-2
hooks:                      # Optional. PreToolUse/PostToolUse/Stop lifecycle hooks.
  PreToolUse:
    - matcher: "Bash"
      hooks:
        - type: command
          command: "./scripts/validate.sh"
---
```

### Field Details

**`name`** -- Unique identifier. Use lowercase-with-hyphens. This is what appears in `Task(agent-name)` for permission rules.

**`description`** -- The single most important field. Claude reads this to decide when to auto-delegate. Be specific about what the agent does and when to use it. Include "Use proactively" if Claude should delegate without being asked.

**`tools`** -- Comma-separated list of allowed tools. If omitted, the agent inherits everything from the main conversation (including MCP tools). Always restrict to the minimum needed.

Available tools: `Read`, `Grep`, `Glob`, `Write`, `Edit`, `Bash`, `WebFetch`, `WebSearch`, `NotebookEdit`, `Task`, plus any MCP tools.

**`disallowedTools`** -- Explicitly deny specific tools. Useful when you want most tools but need to block a few.

**`model`** -- Match model to task complexity:
- `haiku` -- Fast/cheap. Use for search, exploration, simple analysis.
- `sonnet` -- Balanced. Use for code review, moderate analysis.
- `opus` -- Most capable. Use for complex reasoning, architecture decisions.
- `inherit` -- Same as main conversation. Default if omitted.

**`skills`** -- List of skill names (matching filenames in `.claude/skills/` without `.md`). Full skill content is injected into the agent's context at startup. Agents do NOT inherit skills from the parent conversation -- you must list them explicitly.

**`permissionMode`** -- Controls how the agent handles permission prompts:
- `default` -- Standard prompts (recommended for most agents)
- `acceptEdits` -- Auto-accept file edits
- `dontAsk` -- Auto-deny prompts (allowed tools still work)
- `bypassPermissions` -- Skip all checks (use with extreme caution)
- `plan` -- Read-only exploration mode

**`hooks`** -- Lifecycle hooks for hard constraints. Prompts are suggestions; hooks are enforced.
- `PreToolUse` -- Validate before tool execution. Exit code 2 blocks the operation.
- `PostToolUse` -- Run after tool execution (e.g., lint after edits).
- `Stop` -- Run when agent finishes.

## System Prompt (Body)

Everything after the frontmatter `---` becomes the agent's system prompt. The agent receives ONLY this prompt plus basic environment details (working directory, etc.) -- not the full Claude Code system prompt.

### Structure the Body

A well-structured agent body has these sections in order:

1. **Role** -- One sentence. What the agent is and its core mindset.
2. **Input** -- What the agent receives from the caller. Define what's expected so the agent doesn't guess.
3. **Process** -- Numbered steps the agent follows. Reference preloaded skills by name rather than duplicating their content.
4. **Output** -- Exact format of what the agent returns to the caller. Keep it concise -- verbose returns defeat the purpose of context isolation.
5. **Stop condition** -- Explicit instruction on when to stop. Without this, agents explore endlessly and burn tokens.

### Writing Rules

- **Be concise.** The body competes with tool outputs for context space. Put domain knowledge in skills, not the body.
- **Reference skills, don't duplicate.** If a skill is preloaded via the `skills` field, say "Apply the techniques from the X skill" -- don't restate the skill's content.
- **Define clear inputs.** State what the agent expects to receive. If no input is provided, tell it to ask or stop.
- **Define clear outputs.** Specify the return format. Instruct the agent NOT to return full file contents in its summary -- code goes in files, summaries go to the caller.
- **Define stop conditions.** "Stop after completing step N and returning the summary. Do not explore further."
- **Be specific about behavior.** "When invoked: 1. Do X. 2. Do Y." beats "You should generally try to do X."

## Best Practices

### 1. Write Specific Descriptions
Claude delegates based on the `description` field. Generic descriptions cause missed or wrong delegations.

Bad: `"A reviewer"`
Good: `"Expert code reviewer for quality, security, and maintainability. Use proactively after writing or modifying code."`

### 2. Restrict Tools to Minimum Needed
Agents inherit ALL tools if `tools` is omitted. Always set it explicitly.

- Read-only agents: `tools: Read, Grep, Glob, Bash`
- Agents that write code: `tools: Read, Grep, Glob, Write, Edit, Bash`
- Never give `WebFetch`/`WebSearch` unless the agent needs internet access

### 3. Match Model to Task
- `haiku` for search/exploration (fast, cheap)
- `sonnet` for analysis/review (balanced)
- `opus` for complex reasoning (most capable)
- `inherit` when you want it to match whatever the user is running

### 4. Preload Skills Instead of Inlining Knowledge
Use the `skills` field to inject domain knowledge. This keeps the body short and the knowledge reusable across agents.

### 5. Design Focused Agents
Each agent should excel at one specific task. Don't build Swiss Army knife agents. Compose multiple focused agents via chaining instead.

### 6. Use Hooks for Hard Constraints
A prompt saying "don't write files" is a suggestion. A `disallowedTools: Write, Edit` is enforced. A `PreToolUse` hook that validates commands is even stronger. Use the right enforcement level:
- Soft: Prompt instructions (agent may ignore under pressure)
- Medium: `tools` / `disallowedTools` (structurally enforced)
- Hard: `hooks` with validation scripts (programmatically enforced)

### 7. Keep Return Output Concise
Agent output returns to the main conversation. If it's huge, you've defeated the purpose of context isolation. Instruct agents to return summaries, not full content.

## Common Mistakes

| Mistake | Why It's Bad | Fix |
|---------|-------------|-----|
| No frontmatter | Agent has no name, description, or tool restrictions | Always include frontmatter with at least `name` and `description` |
| Vague description | Claude can't decide when to delegate | Be specific: what it does, what triggers it |
| Omitting `tools` | Agent inherits everything, no constraints | Explicitly list required tools |
| Prompts as constraints | "You are read-only" can be ignored | Use `tools`, `disallowedTools`, or hooks |
| No stop condition | Agent explores endlessly, burns tokens | Add explicit "Stop after X" instruction |
| Duplicating skill content in body | Wastes context, drifts from source of truth | Preload via `skills` field, reference by name |
| Verbose return format | Large output floods caller's context | Instruct agent to return concise summary |
| Agent for trivial tasks | Startup overhead (fresh context, no prior conversation) | Only use agents for substantial, isolated work |
| Nested delegation | Subagents cannot spawn other subagents | Chain agents from main conversation instead |

## Agent vs Skill vs Command Decision

- Need **isolation, tool restrictions, or parallel execution**? Use an Agent.
- Need **domain knowledge injected into the current conversation**? Use a Skill.
- Have stuff in `.claude/commands/`? Still works, but it's deprecated. Migrate to skills.

## Template

```markdown
---
name: agent-name
description: What it does and when to use it. Be specific.
tools: Read, Grep, Glob
model: inherit
skills:
  - relevant-skill-1
  - relevant-skill-2
---

You are a [role]. [Core mindset in one sentence.]

## Input

You receive [what the caller passes]. If not provided, [what to do].

## Process

1. **[Step name].** [What to do. Reference skills by name.]
2. **[Step name].** [Next action.]
3. **[Step name].** [Final action.]

## Output

Return a concise summary:
- [What to include]
- [What to include]

Do NOT return [what to exclude].

## Stop Condition

Stop after completing step N and returning the summary. Do not explore further.
```
