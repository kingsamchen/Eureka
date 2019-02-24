package com.kc.listview.app;

import android.content.Context;
import android.media.Image;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import org.w3c.dom.Text;

import java.util.List;
import java.util.zip.Inflater;

/**
 * @ 0xCCCCCCCC
 */
public class FruitAdapter extends ArrayAdapter<Fruit> {
    private int mLayoutResource;

    public FruitAdapter(Context context, int layoutResource, List<Fruit> fruits) {
        super(context, layoutResource, fruits);
        mLayoutResource = layoutResource;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View fruitItemView;
        ViewHolder holder;
        if (convertView == null) {
            fruitItemView = LayoutInflater.from(getContext()).inflate(mLayoutResource, null);
            holder = new ViewHolder();
            holder.textView = (TextView)fruitItemView.findViewById(R.id.fruit_name);
            holder.imageView = (ImageView)fruitItemView.findViewById(R.id.fruit_avatar);
            fruitItemView.setTag(holder);
        } else {
            fruitItemView = convertView;
            holder = (ViewHolder)fruitItemView.getTag();
        }

        Fruit fruit = getItem(position);
        holder.textView.setText(fruit.getName());
        holder.imageView.setImageResource(fruit.getImageId());

        return fruitItemView;
    }

    public static class ViewHolder {
        public TextView textView;
        public ImageView imageView;
    }
}
