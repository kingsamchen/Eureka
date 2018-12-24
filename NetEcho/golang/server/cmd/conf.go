package main

import (
	"flag"

	"github.com/BurntSushi/toml"
	"github.com/pkg/errors"
)

var (
	confPath string
	Conf     = &Config{}
)

type Config struct {
	Port uint16
}

func init() {
	flag.StringVar(&confPath, "conf", "server_conf.toml", "Path of toml configuration")
}

func Init() error {
	if confPath == "" {
		return errors.New("Empty conf path")
	}

	_, err := toml.DecodeFile(confPath, Conf)
	return err
}
