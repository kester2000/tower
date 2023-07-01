#include <iostream>

#include "tower.h"

using namespace std;

int main()
{
    // init(
    //     "0,B-R-20201219-170046-206E9C-SYSTEM,test7433,1608368485792,Lv_B_206E9C,,\r\n$$\r\n250 10 5 0 0 3 1 0 0 3 "
    //     "3\r\n$$\r\na3 a3 a3 a3 a3 a3 a3 a3 a3 a3 a3 a3 a3\r\na3 a0 c1 i2 a0 c4 a3 a0 i3 c6 a3 c6 a3\r\na3 h0 a3 a3
    //     a3 " "i3 a3 a0 a3 j0 a3 j8 a3\r\na3 j0 h0 a0 a3 c6 j1 a0 j8 a0 a0 h0 a3\r\na3 c0 a3 j1 a3 a3 a3 a0 a3 a0 a3
    //     a0 " "a3\r\na3 a0 a0 j0 a3 c7 i3 h0 i3 a0 a3 a0 a3\r\na3 a3 a3 a0 a3 a3 a3 c7 a3 h0 a3 a0 a3\r\na3 h0 i0 a0
    //     a3 c0 " "a0 a0 a3 c6 a3 d1 a3\r\na3 h0 a3 a3 a3 a0 a3 a3 a3 h1 a3 a0 a3\r\na3 h0 l4 c4 a3 a0 a0 j0 h1 i7 c7
    //     i2 " "a3\r\na3 a0 a3 a3 a3 a0 a3 a0 a3 l4 a3 j8 a3\r\na3 c0 a0 d1 a0 c0 a3 a0 a0 c6 h1 d0 a3\r\na3 a3 a3 a3
    //     a3 a3 " "a3 a3 a3 a3 a3 a3 " "a3\r\n#\r\n$$\r\nmapcode\u003di0\r\nprefix\u003dgreen_"
    //     "slime\r\nname\u003d小绿\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d30\r\nattack\u003d6\r\ndefend"
    //     "\u003d5\r\ncoins\u003d11\r\nexp\u003d3\r\nsp\u003d0\r\n\r\nmapcode\u003di2\r\nprefix\u003dblack_"
    //     "slime\r\nname\u003d小灰\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d60\r\nattack\u003d5\r\ndefend"
    //     "\u003d5\r\ncoins\u003d10\r\nexp\u003d6\r\nsp\u003d2\r\n\r\nmapcode\u003di3\r\nprefix\u003dslime_"
    //     "king\r\nname\u003d史莱姆王\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d60\r\nattack\u003d22\r\ndefe"
    //     "nd\u003d5\r\ncoins\u003d27\r\nexp\u003d6\r\nsp\u003d0\r\n\r\nmapcode\u003dj0\r\nprefix\u003dgray_"
    //     "witcher\r\nname\u003d见习巫师\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d35\r\nattack\u003d6\r\nde"
    //     "fend\u003d5\r\ncoins\u003d11\r\nexp\u003d4\r\nsp\u003d2\r\n\r\nmapcode\u003dj1\r\nprefix\u003dred_"
    //     "witcher\r\nname\u003d巫师\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d195\r\nattack\u003d9\r\ndefen"
    //     "d\u003d4\r\ncoins\u003d13\r\nexp\u003d20\r\nsp\u003d2\r\n\r\nmapcode\u003dj8\r\nprefix\u003dstoneman\r\nname"
    //     "\u003d石人\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d15\r\nattack\u003d19\r\ndefend\u003d0\r\ncoi"
    //     "ns\u003d19\r\nexp\u003d2\r\nsp\u003d3\r\n\r\nmapcode\u003dl4\r\nprefix\u003dblack_stoneman\r\nname\u003d精英
    //     "
    //     "石人\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d20\r\nattack\u003d35\r\ndefend\u003d0\r\ncoins"
    //     "\u003d35\r\nexp\u003d2\r\nsp\u003d3\r\n\r\nmapcode\u003di7\r\nprefix\u003dBOSS\r\nname\u003dBOSS！\r\nbfEvents"
    //     "\u003d伟大的勇士，等你好久了\r\nafEvents\u003d我居然被打败了！\r\nhp\u003d410\r\nattack\u003d38\r\ndefend"
    //     "\u003d11\r\ncoins\u003d0\r\nexp\u003d9999\r\nsp\u003d0\r\n\r\n#\r\n$$\r\n3 3 0 75 200 2000 500 10 10");
    init(
        "0,S-R-20201023-133824-2D62BC-SYSTEM,test7073,1603431874166,Lv_S_2D62BC,,\r\n$$\r\n500 10 5 0 0 3 1 0 0 10 "
        "12\r\n$$\r\na0 a0 a0 a0 a0 a0 c1 c0 a0 c7 h0 c7 h0\r\na0 a3 a3 a3 a3 a3 a3 a3 a3 a3 d0 a3 a0\r\nl1 a0 l3 a3 "
        "h0 a0 a0 h1 h0 a0 a0 h0 h0\r\na3 a3 a0 a3 a0 a3 a0 a3 a0 a3 d0 a3 h1\r\nc2 a3 a0 a3 a0 a3 a0 a0 c0 a3 l2 a0 "
        "a0\r\nk4 a3 a0 a3 c0 a3 a3 a3 a3 a3 a3 a3 k3\r\na0 h0 a0 h1 d0 h0 i7 a3 r3 k7 l1 a3 c4\r\nc0 a3 l2 a3 a3 a3 "
        "a3 a3 a3 a3 a0 a3 a3\r\nh0 a3 a0 a0 c6 a3 c1 h1 c7 a3 c6 a0 a0\r\nh0 a3 a3 a3 a3 a3 a0 a3 h2 a3 a3 a3 "
        "a0\r\na0 a3 a0 a0 a0 l1 d0 c7 a0 a3 l1 a0 a0\r\na0 a3 a0 a3 h1 a3 a3 a3 c7 a3 a0 a3 j7\r\nk3 c6 a0 a3 c4 i2 "
        "a0 a0 a0 a0 a0 a3 "
        "c2\r\n#\r\n$$\r\nmapcode\u003di2\r\nprefix\u003dblack_"
        "slime\r\nname\u003d小灰\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d75\r\nattack\u003d25\r\ndefend"
        "\u003d5\r\ncoins\u003d30\r\nexp\u003d8\r\nsp\u003d2\r\n\r\nmapcode\u003dj7\r\nprefix\u003dorc_"
        "guard\r\nname\u003d兽人卫兵\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d110\r\nattack\u003d24\r\nde"
        "fend\u003d12\r\ncoins\u003d36\r\nexp\u003d11\r\nsp\u003d21\r\n\r\nmapcode\u003dk3\r\nprefix\u003dswordman\r\nn"
        "ame\u003d双手剑士\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d70\r\nattack\u003d21\r\ndefend\u003d4"
        "\r\ncoins\u003d25\r\nexp\u003d7\r\nsp\u003d4\r\n\r\nmapcode\u003dk4\r\nprefix\u003dnetherguard\r\nname\u003d冥"
        "斗士\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d120\r\nattack\u003d21\r\ndefend\u003d8\r\ncoins"
        "\u003d29\r\nexp\u003d12\r\nsp\u003d10050\r\n\r\nmapcode\u003dk7\r\nprefix\u003dnether_"
        "warrior\r\nname\u003d影剑士\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d90\r\nattack\u003d21\r\ndef"
        "end\u003d0\r\ncoins\u003d21\r\nexp\u003d9\r\nsp\u003d10150\r\n\r\nmapcode\u003dl1\r\nprefix\u003dyellow_"
        "slime\r\nname\u003d变异小黄\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d60\r\nattack\u003d6\r\ndefe"
        "nd\u003d5\r\ncoins\u003d11\r\nexp\u003d6\r\nsp\u003d40025\r\n\r\nmapcode\u003dl2\r\nprefix\u003dphantom_"
        "ske\r\nname\u003d变异骷髅\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d160\r\nattack\u003d23\r\ndefe"
        "nd\u003d1\r\ncoins\u003d24\r\nexp\u003d16\r\nsp\u003d40025\r\n\r\nmapcode\u003dl3\r\nprefix\u003dphantom_"
        "bat\r\nname\u003d变异蝙蝠\r\nbfEvents\u003dnull\r\nafEvents\u003dnull\r\nhp\u003d95\r\nattack\u003d13\r\ndefen"
        "d\u003d9\r\ncoins\u003d22\r\nexp\u003d10\r\nsp\u003d40025\r\n\r\nmapcode\u003di7\r\nprefix\u003dBOSS\r\nname"
        "\u003dBOSS！\r\nbfEvents\u003d伟大的勇士 "
        "，等你好久了\r\nafEvents\u003d我居然被打败了！\r\nhp\u003d910\r\nattack\u003d43\r\ndefend\u003d19\r\ncoins"
        "\u003d0\r\nexp\u003d9999\r\nsp\u003d0\r\n\r\n#\r\n$$\r\n3 3 0 75 200 2000 500 10 10");
    char buff[1024] = {0};
    spfa(buff);
    return 0;
}