package config

import (
    "fmt"
    "os"
    "path/filepath"

    "gopkg.in/yaml.v3"
)

// LoadConfig loads configuration from a YAML file
func LoadConfig(configPath string) (*Config, error) {
    // Expand path if relative
    absPath, err := filepath.Abs(configPath)
    if err != nil {
        return nil, fmt.Errorf("failed to resolve config path: %w", err)
    }

    // Read file
    data, err := os.ReadFile(absPath)
    if err != nil {
        return nil, fmt.Errorf("failed to read config file: %w", err)
    }

    // Parse YAML
    var config Config
    if err := yaml.Unmarshal(data, &config); err != nil {
        return nil, fmt.Errorf("failed to parse config YAML: %w", err)
    }

    // Apply defaults and validate
    if err := applyDefaults(&config); err != nil {
        return nil, fmt.Errorf("failed to apply defaults: %w", err)
    }

    if err := validateConfig(&config); err != nil {
        return nil, fmt.Errorf("config validation failed: %w", err)
    }

    return &config, nil
}

// applyDefaults applies default values to configuration
func applyDefaults(config *Config) error {
    // Apply global defaults
    if config.Global.LogLevel == "" {
        config.Global.LogLevel = "info"
    }
    if config.Global.DataDir == "" {
        config.Global.DataDir = "./data"
    }
    if config.Global.WebPort == 0 {
        config.Global.WebPort = 8080
    }

    // Apply job defaults
    for i := range config.Jobs {
        job := &config.Jobs[i]
        if job.Options.BackupType == "" {
            job.Options.BackupType = "full"
        }
        if job.Options.Compression == "" {
            job.Options.Compression = "gzip"
        }
        if job.Retention.KeepDaily == 0 {
            job.Retention.KeepDaily = 7
        }
        if job.Retention.KeepWeekly == 0 {
            job.Retention.KeepWeekly = 4
        }
        if job.Retention.KeepMonthly == 0 {
            job.Retention.KeepMonthly = 6
        }
    }

    return nil
}

// validateConfig validates the configuration
func validateConfig(config *Config) error {
    // Validate global config
    if config.Global.WebPort < 1 || config.Global.WebPort > 65535 {
        return fmt.Errorf("invalid web port: %d", config.Global.WebPort)
    }

    // Validate jobs
    jobNames := make(map[string]bool)
    for i, job := range config.Jobs {
        if job.Name == "" {
            return fmt.Errorf("job %d: name is required", i)
        }

        if jobNames[job.Name] {
            return fmt.Errorf("duplicate job name: %s", job.Name)
        }
        jobNames[job.Name] = true

        if job.Source.Type == "" {
            return fmt.Errorf("job %s: source type is required", job.Name)
        }

        if job.Destination.Type == "" {
            return fmt.Errorf("job %s: destination type is required", job.Name)
        }

        if len(job.Source.Paths) == 0 {
            return fmt.Errorf("job %s: at least one source path is required", job.Name)
        }
    }

    return nil
}