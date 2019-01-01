package main

import (
	"errors"
	"flag"
	"github.com/BurntSushi/toml"
)

var (
	confPath string
	Conf = &Config{}
)

type Config struct {
	Port uint16
}

func init() {
	flag.StringVar(&confPath, "conf", "", "server conf path")
}

// Init inits the server with configurations.
func Init() error {
	if confPath == "" {
		return errors.New("server conf not found")
	}

	_, err := toml.DecodeFile(confPath, Conf)
	return err
}
