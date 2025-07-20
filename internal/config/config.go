package config

import (
    "time"
)

// Config represents the main configuration structure
type Config struct {
    Global GlobalConfig `yaml:"global"`
    Jobs   []JobConfig  `yaml:"jobs"`
}

// GlobalConfig contains global settings
type GlobalConfig struct {
    LogLevel    string `yaml:"log_level" default:"info"`
    LogFile     string `yaml:"log_file"`
    DataDir     string `yaml:"data_dir" default:"./data"`
    WebPort     int    `yaml:"web_port" default:"8080"`
    WebEnabled  bool   `yaml:"web_enabled" default:"false"`
}

// JobConfig represents a backup job configuration
type JobConfig struct {
    Name        string            `yaml:"name"`
    Description string            `yaml:"description"`
    Enabled     bool              `yaml:"enabled" default:"true"`
    Schedule    string            `yaml:"schedule"` // cron expression
    Source      SourceConfig      `yaml:"source"`
    Destination DestinationConfig `yaml:"destination"`
    Options     BackupOptions     `yaml:"options"`
    Retention   RetentionConfig   `yaml:"retention"`
}

// SourceConfig defines what to backup
type SourceConfig struct {
    Type    string            `yaml:"type"` // "files", "database", etc.
    Paths   []string          `yaml:"paths"`
    Exclude []string          `yaml:"exclude"`
    Options map[string]string `yaml:"options"`
}

// DestinationConfig defines where to backup
type DestinationConfig struct {
    Type        string            `yaml:"type"` // "local", "s3", "gcs", etc.
    Path        string            `yaml:"path"`
    Credentials map[string]string `yaml:"credentials"`
    Options     map[string]string `yaml:"options"`
}

// BackupOptions contains backup-specific settings
type BackupOptions struct {
    BackupType   string `yaml:"backup_type" default:"full"` // "full", "incremental"
    Compression  string `yaml:"compression" default:"gzip"` // "none", "gzip", "lz4"
    Encryption   bool   `yaml:"encryption" default:"false"`
    Verification bool   `yaml:"verification" default:"true"`
}

// RetentionConfig defines how long to keep backups
type RetentionConfig struct {
    KeepDaily   int           `yaml:"keep_daily" default:"7"`
    KeepWeekly  int           `yaml:"keep_weekly" default:"4"`
    KeepMonthly int           `yaml:"keep_monthly" default:"6"`
    MaxAge      time.Duration `yaml:"max_age" default:"2160h"` // 90 days
}