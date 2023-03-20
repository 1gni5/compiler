FROM alpine:3.17.2 AS build

# Install compilation tools
RUN apk add --no-cache build-base cmake clang make

# Install C++ headers
RUN wget 'https://www.antlr.org/download/antlr4-cpp-runtime-4.12.0-source.zip' -P /tmp
RUN unzip /tmp/antlr4-cpp-runtime-4.12.0-source.zip -d /tmp && \
    cd /tmp && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install


FROM alpine:3.17.2 as runtime

# # Install packages
RUN apk add --no-cache zsh openjdk11-jre
RUN wget https://www.antlr.org/download/antlr-4.12.0-complete.jar -P /usr/local/lib

# Set environment 
ENV CLASSPATH=".:/usr/local/lib/antlr-4.12.0-complete.jar:$CLASSPATH"
RUN echo "alias antlr4='java -jar /usr/local/lib/antlr-4.12.0-complete.jar'" > /root/.zshrc
COPY --from=build /usr/local/lib/ /usr/local/lib/
COPY --from=build /usr/local/include/ /usr/local/include/

CMD ["/bin/zsh"]