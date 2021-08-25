import platform
import subprocess
import telebot
import os
import time

config_filename = "../test_000.cfg"

TEXTS = {}
# loading texts
file = open('../the_ark_bot/media/texts.txt', encoding="utf-8")
texts = file.read()
texts = texts.split("--")
texts.remove("")
for i in range(len(texts) // 2):
    TEXTS[texts[2 * i]] = texts[2 * i + 1]
file.close()


def update_config_file(data):
# Updates information in config file by data
    file = open(config_filename, "w")
    file.write(data)
    file.close()


TOKEN = '1975742331:AAGLCpQXH1KXVgtYx8v3JApsky1_4D53z_U'    

bot = telebot.TeleBot(TOKEN, parse_mode=None)
BOOL_CHANGE_PARAMETERS = False

file = open('../test_000.cfg')
parameters = file.read()
parameters_list = parameters.split()
file.close()

BOOL_READ_FROM_COUT = True

@bot.message_handler(commands=['start'])
def introduction(message):
    bot.send_message(chat_id=message.chat.id, text=TEXTS["INTRODUCTION"], parse_mode="Markdown")


@bot.message_handler(commands=['help'])
def send_instructions(message):
    bot.send_message(chat_id=message.chat.id, text="Waiting for help...")


############################
# Information about services
#
#
@bot.message_handler(commands=['technical'])
def technical_info(message):
    bot.send_message(chat_id=message.chat.id, text=TEXTS["TECHNICAL_INFO"], parse_mode="Markdown")

@bot.message_handler(commands=['biological'])
def biological_info(message):
    bot.send_message(chat_id=message.chat.id, text=TEXTS["BIOLOGICAL_INFO"], parse_mode="Markdown")

@bot.message_handler(commands=['medical'])
def medical_info(message):
    bot.send_message(chat_id=message.chat.id, text=TEXTS["MEDICAL_INFO"], parse_mode="Markdown")

@bot.message_handler(commands=['navigation'])
def navigation_info(message):
    bot.send_message(chat_id=message.chat.id, text=TEXTS["NAVIGATION_INFO"], parse_mode="Markdown")

@bot.message_handler(commands=['emergency'])
def emergency_info(message):
    bot.send_message(chat_id=message.chat.id, text=TEXTS["EMERGENCY_INFO"], parse_mode="Markdown")

@bot.message_handler(commands=['social'])
def social_info(message):
    bot.send_message(chat_id=message.chat.id, text=TEXTS["SOCIAL_INFO"], parse_mode="Markdown")

@bot.message_handler(commands=['general'])
def general_info(message):
    bot.send_message(chat_id=message.chat.id, text=TEXTS["GENERAL_INFO"], parse_mode="Markdown")
#
#
############################

@bot.message_handler(commands=['parameters', 'new_flight'])
def show_parameters(message):
    bot.send_message(chat_id=message.chat.id, text="Текущие параметры:\n\n" + parameters + "\nЧтобы внести изменения отправьте /change_parameters")

@bot.message_handler(commands=['change_parameters'])
def change_parameters1(message):
    global BOOL_CHANGE_PARAMETERS
    BOOL_CHANGE_PARAMETERS = True
    bot.send_message(chat_id=message.chat.id, text="Введите *имя параметра* *новое значение*")

@bot.message_handler(func=lambda message: BOOL_CHANGE_PARAMETERS)
def change_parameters2(message):
    global BOOL_CHANGE_PARAMETERS, parameters, parameters_list
    BOOL_CHANGE_PARAMETERS = False
    message_text = message.text.split(" ")
    if message_text[0] in parameters_list:
        parameters = parameters.replace(message_text[0] + " " + str(parameters_list[parameters_list.index(message_text[0]) + 1]), message_text[0] + " " + message_text[1])
        parameters_list[parameters_list.index(message_text[0]) + 1] = message_text[1]
        bot.send_message(chat_id=message.chat.id, text="Новые параметры:\n\n" + parameters)
        update_config_file(parameters)
    else:
        bot.send_message(chat_id=message.chat.id, text="Такого параметра нет.")


@bot.message_handler(commands=['go'])
def flight(message):
    global BOOL_READ_FROM_COUT
    bot.send_message(chat_id=message.chat.id, text=TEXTS["STARTING FLIGHT"])

    current_os = platform.system()
    executable_path = '../build/the_ark'

    if current_os == "Windows":
        executable_path = '..\\cmake-build-debug\\the_ark.exe'

    process = subprocess.Popen(executable_path, stdout=subprocess.PIPE, stdin=subprocess.PIPE, text=1)
    while process.poll() == None:
        output = ""
        a = ""
        if BOOL_READ_FROM_COUT:
            output = process.stdout.readline()
            a = output
        if len(output) != 0:
            output = process.stdout.readline()
            a += output
            output = process.stdout.readline()
            a += output
        if len(a) != 0:
            bot.send_message(chat_id=message.chat.id, text=a, reply_markup=telebot.types.ForceReply())
            BOOL_READ_FROM_COUT = False

            @bot.message_handler(content_types=['text'])
            def message_input_step(message):
                inp = message.text
                process.stdin.write(inp + "\n")
                process.stdin.flush()
                time.sleep(1)
                global BOOL_READ_FROM_COUT
                if process.poll() == None:
                    BOOL_READ_FROM_COUT = True

            bot.register_next_step_handler(message, message_input_step)

    BOOL_READ_FROM_COUT = True
    bot.send_message(chat_id=message.chat.id, text="Полёт завершён.")
    os.system('python plots.py')
    plot_population = open('Population.png', 'rb')
    plot_services = open('Services.png', 'rb')
    plot_resources = open('Resources.png', 'rb')
    bot.send_message(chat_id=message.chat.id, text=TEXTS["SENDING PLOTS"])
    bot.send_photo(chat_id=message.chat.id, photo=plot_population)
    bot.send_photo(chat_id=message.chat.id, photo=plot_services)
    bot.send_photo(chat_id=message.chat.id, photo=plot_resources)

@bot.message_handler(func=lambda message: True)
def other_messages(message):
    bot.send_message(chat_id=message.chat.id, text="?")


bot.polling()
