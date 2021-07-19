FROM pandoc/latex:2.9

LABEL maintainer="TieWay59" 

RUN tlmgr install adjustbox babel-german background bidi collectbox csquotes everypage filehook footmisc footnotebackref framed fvextra letltxmacro ly1 mdframed mweights needspace pagecolor sourcecodepro sourcesanspro titling ucharcat ulem unicode-math upquote xecjk xurl zref ctex \
    && apk add --update font-noto-cjk \
    && fc-cache -f
