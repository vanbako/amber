# Ollama API Notes

This note captures the invocation pattern for running local Ollama models via `curl`. It mirrors the workflow we used for VeriReason so we can reuse it without relearning the quirks of PowerShell JSON handling.

## Prerequisites
- Ollama daemon running on the default host (`http://127.0.0.1:11434`).
- Prompt text saved to disk (for large prompts, build a single concatenated file first).
- Model already pulled locally if it lives in a remote registry.

### Current Model Catalog
- `hf.co/mradermacher/VeriReason-Qwen2.5-7b-RTLCoder-Verilog-GRPO-reasoning-tb-i1-GGUF:Q6_K`

## Prepare the Prompt Bundle
If the prompt lives in `build\verireason_prompt.txt`, ensure it already contains any contextual data (module listings, etc.). The file must be plain text because the API expects the prompt as a JSON string.

## Build the JSON Request
Use PowerShell to read the prompt as a raw string and encode it into the JSON payload. Reading via `Get-Content` without `-Raw` returns an array, so always prefer the `[System.IO.File]::ReadAllText` helper.

```powershell
$prompt = [System.IO.File]::ReadAllText('build\verireason_prompt.txt')
$request = @{
    model  = 'hf.co/mradermacher/VeriReason-Qwen2.5-7b-RTLCoder-Verilog-GRPO-reasoning-tb-i1-GGUF:Q6_K'
    prompt = $prompt
    stream = $false   # set true to stream partial tokens
}
$request | ConvertTo-Json -Depth 5 | Set-Content -Path 'build\verireason_request.json'
```

Key reminders:
- `ConvertTo-Json` on a `PSCustomObject` that still contains PowerShell objects (e.g., from `Get-Content` without `-Raw`) yields nested metadata; avoid that by using `ReadAllText`.
- Keep `stream = $false` when you want the entire reply captured in one block; streaming is handy for interactive terminals but harder to save to disk.

## Execute the Request with `curl`
Post the JSON file directly with `--data-binary` so the newline formatting is preserved.

```powershell
curl.exe -s ^
  -X POST http://127.0.0.1:11434/api/generate ^
  -H "Content-Type: application/json" ^
  --data-binary "@build\verireason_request.json" |
  Set-Content -Path 'build\verireason_output_curl.json'
```

The response is a single JSON object. When `stream = true`, Ollama emits a JSON object per line terminated by a final `{ "done": true }`; in that case use `Add-Content` instead of `Set-Content` to capture the whole sequence.

## Parsing the Response
Read the response back into PowerShell and inspect the `response` field:

```powershell
$resp = Get-Content -Raw -Path 'build\verireason_output_curl.json' | ConvertFrom-Json
$resp.response
```

If you need the low-level token context, keep the raw file around; Ollama returns a `context` array that can be reused for follow-up calls by adding it to the JSON payload.
