# Makefile - Build rules for the hybrid desktop GTK application

APP_NAME=hybrid-desktop
PREFIX?=/usr/local
BINDIR=$(PREFIX)/bin
DATADIR=$(PREFIX)/share/$(APP_NAME)
DESKTOPDIR?=/usr/share/applications
CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0) -Iinclude -Wall -Wextra -O2
LDFLAGS=$(shell pkg-config --libs gtk+-3.0)
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: $(APP_NAME)

$(APP_NAME): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: all
	# バイナリを $(BINDIR) に配置
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(APP_NAME) $(DESTDIR)$(BINDIR)/$(APP_NAME)
	# テーマなどのリソースを $(DATADIR) に配置
	install -d $(DESTDIR)$(DATADIR)
	install -m 644 data/theme.css $(DESTDIR)$(DATADIR)/theme.css
	# デスクトップエントリをシステムメニューに配置
	install -d $(DESTDIR)$(DESKTOPDIR)
	install -m 644 data/org.example.hybriddesktop.desktop $(DESTDIR)$(DESKTOPDIR)/org.example.hybriddesktop.desktop
	@echo "# ~/.config/autostart/ に .desktop をコピーすると自動起動できます。"

uninstall:
	-rm -f $(DESTDIR)$(BINDIR)/$(APP_NAME)
	-rm -f $(DESTDIR)$(DESKTOPDIR)/org.example.hybriddesktop.desktop
	-rm -rf $(DESTDIR)$(DATADIR)

clean:
	rm -f $(OBJ) $(APP_NAME)

.PHONY: all clean install uninstall
