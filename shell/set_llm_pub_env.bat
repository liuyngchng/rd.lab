@echo off
echo Setting PUBLIC environment variables...

:: Check if PUB_BASE_URL is set
if "%PUB_BASE_URL%"=="" (
    echo [ERROR] PUB_BASE_URL environment variable is not set!
    echo Please set it using: set PUB_BASE_URL=your_url
    echo Or set it permanently using: setx PUB_BASE_URL your_url
    pause
    exit /b 1
)

:: Check if PUB_API_KEY is set
if "%PUB_API_KEY%"=="" (
    echo [ERROR] PUB_API_KEY environment variable is not set!
    echo Please set it using: set PUB_API_KEY=your_api_key
    echo Or set it permanently using: setx PUB_API_KEY your_api_key
    pause
    exit /b 1
)

:: Set Anthropic environment variables
set ANTHROPIC_BASE_URL=%PUB_BASE_URL%
set ANTHROPIC_AUTH_TOKEN=%PUB_API_KEY%
set ANTHROPIC_MODEL=deepseek-v4-pro[1m]
set ANTHROPIC_DEFAULT_OPUS_MODEL=deepseek-v4-pro[1m]
set ANTHROPIC_DEFAULT_SONNET_MODEL=deepseek-v4-pro[1m]
set ANTHROPIC_DEFAULT_HAIKU_MODEL=deepseek-v4-flash
set CLAUDE_CODE_SUBAGENT_MODEL=deepseek-v4-flash
set CLAUDE_CODE_EFFORT_LEVEL=max

echo.
echo ========================================
echo Environment variables set successfully!
echo ========================================
echo ANTHROPIC_BASE_URL=%ANTHROPIC_BASE_URL%
echo ANTHROPIC_AUTH_TOKEN=%ANTHROPIC_AUTH_TOKEN%
echo ANTHROPIC_MODEL=%ANTHROPIC_MODEL%
echo ANTHROPIC_DEFAULT_OPUS_MODEL=%ANTHROPIC_DEFAULT_OPUS_MODEL%
echo ANTHROPIC_DEFAULT_SONNET_MODEL=%ANTHROPIC_DEFAULT_SONNET_MODEL%
echo ANTHROPIC_DEFAULT_HAIKU_MODEL=%ANTHROPIC_DEFAULT_HAIKU_MODEL%
echo CLAUDE_CODE_SUBAGENT_MODEL=%CLAUDE_CODE_SUBAGENT_MODEL%
echo CLAUDE_CODE_EFFORT_LEVEL=%CLAUDE_CODE_EFFORT_LEVEL%
echo ========================================
echo.
