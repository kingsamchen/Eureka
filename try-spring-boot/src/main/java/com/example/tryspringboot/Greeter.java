package com.example.tryspringboot;

public class Greeter {
    private final long id;
    private final String content;

    public Greeter(long id, String content) {
        this.id = id;
        this.content = content;
    }

    public long getId() {
        return this.id;
    }

    public String getContent() {
        return this.content;
    }
}
