package com.example.tryspringboot;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import java.util.concurrent.atomic.AtomicLong;

@RestController
public class GreetingController {
    private final AtomicLong counter = new AtomicLong();
    private static final String TEMPLATE = "Hello, %s!";

    @GetMapping("/hello-world")
    @ResponseBody
    public Greeter sayHello(@RequestParam(value = "name", defaultValue = "Stranger", required = false) String name) {
        return new Greeter(counter.incrementAndGet(), String.format(TEMPLATE, name));
    }
}
