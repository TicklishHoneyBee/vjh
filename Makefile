include Makefile.inc

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CD) $(BUILD_DIR); $(CC) $(CLIBS) -o ../$(TARGET) $(OBJS)

fresh: clean all

distclean:
	$(RM) -f $(BUILD_DIR)/*.o

clean: distclean
	$(RM) -f $(TARGET)*

fresh: clean all

release: all distclean
	mkdir -p install
	cp $(TARGET) install/$(TARGET)
	cp README* install/.
	cp LICENSE install/.
	@echo "Archiving Game Data"
	tar cf install/$(TARGET).tar images sounds fonts
	@echo "Creating Install Files"
	$(CD) install; tar czf ../$(TARGET).tar.gz ./*
	@echo "Generating Installer"
	cat xinstall.sh $(TARGET).tar.gz > $(TARGET)-x86.run
	@echo "Cleaning Up"
	rm -f $(TARGET).tar.gz
	rm -f install/*
	rm -f $(TARGET)
	@echo "Created Installer $(TARGET)-x86.run"

install:
	@touch $(BIN_DIR)/$(TARGET)
	@echo "#!/bin/sh" > $(BIN_DIR)/$(TARGET)
	@echo "cd $(TARGET_DIR)" >> $(BIN_DIR)/$(TARGET)
	@echo "./voyager" >> $(BIN_DIR)/$(TARGET)
	@chmod a+x $(BIN_DIR)/$(TARGET)
	@mkdir -p $(TARGET_DIR)
	cp $(TARGET) $(TARGET_DIR)/$(TARGET)
	cp -R images $(TARGET_DIR)/.
	cp -R fonts $(TARGET_DIR)/.
	cp -R sounds $(TARGET_DIR)/.

uninstall:
	$(RM) -f $(BIN_DIR)$(TARGET)


%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

