package main

import (
	"fmt"
	"os"

	"gong/internal/config"
	"github.com/spf13/cobra"
)

var (
	configFile string
	fg *config.Config
)

var rootCmd = &cobra.Command{
	Use: "gong",
	Short: "Fast, automated backup utility",
	Long: "Fast, automated backup and cronjob utilitiy written in Go. Complete with a cli and web interface, compatible with a wide range of local and cloud storage services to your hardrive or from S3 to OneDrive.",
	PersistentPreRunE: func(cmd * cobra.Command, args[]string) error {
		_, err := config.LoadConfig(configFile)
		if err != nil {
			return fmt.Errorf("failed to load config: %w", err)
		}
		return nil
	},
}

var versionCmd = &cobra.Command{
	Use: "version",
	Short: 
}