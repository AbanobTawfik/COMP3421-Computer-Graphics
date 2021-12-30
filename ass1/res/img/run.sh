#!/bin/bash
for f in froge/*.png;
do
    # echo $f
    new=$(echo $f | sed 's/frame//' | sed 's/\_//g' | sed 's/delay.*\.png/.png/' | sed 's/\/0/\//')
    # new=$(echo $f | sed 's/frame//' | sed 's/\_//g' | sed 's/delay.*\.png/.png/' | sed 's/\/0/\//' | sed 's/\/0/\//')
    # new=$(echo $new | sed 's/coffindance/coffin_dance/')
    # new=$(echo $new | sed 's/\.png//' | sed 's/dance\/a\///')
    # new=$(expr $new + 273)
    # new=$(echo dance/$new.png)
    mv $f $new    
    echo $new
done

    auto froge_width = 0.20;
    auto froge_height = 0.45;
    std::vector<vertex_t> froge_vertex = std::vector<vertex_t>{
            {{froge_width, froge_height, 0, 1}, {1, 1}},      // top-right
            {{froge_width, -froge_height, 0, 1}, {1, 0}},        // bottom-right
            {{-froge_width, -froge_height, 0, 1}, {0, 0}},             // bottom-left

            {{froge_width, froge_height, 0, 1}, {1, 1}},           // top-right
            {{-froge_width, -froge_height, 0, 1}, {0, 0}},           // bottom-left
            {{-froge_width, froge_height, 0, 1}, {0, 1}},           // top-left
    };