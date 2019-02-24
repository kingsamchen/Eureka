package com.kc.listview.app;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.*;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        List<Fruit> fruits = new ArrayList<Fruit>();
        fruits.add(new Fruit("Apple", R.drawable.apple_pic));
        fruits.add(new Fruit("Banana", R.drawable.banana_pic));
        fruits.add(new Fruit("Cherry", R.drawable.cherry_pic));
        fruits.add(new Fruit("Grape", R.drawable.grape_pic));
        fruits.add(new Fruit("Mango", R.drawable.mango_pic));
        fruits.add(new Fruit("Orange", R.drawable.orange_pic));
        fruits.add(new Fruit("Pear", R.drawable.pear_pic));
        fruits.add(new Fruit("Pineapple", R.drawable.pineapple_pic));
        fruits.add(new Fruit("Strawberry", R.drawable.strawberry_pic));
        fruits.add(new Fruit("Watermelon", R.drawable.watermelon_pic));

        FruitAdapter fruitsAdapter = new FruitAdapter(MainActivity.this, R.layout.fruit_item, fruits);
        ListView fruitView = (ListView)findViewById(R.id.list_view);
        fruitView.setAdapter(fruitsAdapter);
        fruitView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                FruitAdapter.ViewHolder holder = (FruitAdapter.ViewHolder)view.getTag();
                Toast.makeText(MainActivity.this, holder.textView.getText(), Toast.LENGTH_SHORT).show();
            }
        });

    }
}
