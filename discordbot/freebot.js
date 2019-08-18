const { Client, RichEmbed } = require("discord.js");
const client = new Client();

client.on('ready', () => {
	console.log(`Залогинился под ${client.user.tag}!`);
});

client.on('guildMemberAdd', member => {
	const channel = member.guild.channels.find(ch => ch.name === 'family');
	if (!channel) return;
	channel.send(`Добро пожаловать на сервер, ${member}`);
});

client.on('message', message => {
	if (!message.guild) return;

	if (message.content.startsWith('!kick')) {
		const user = message.mentions.users.first();
		if (user) {
			const member = message.guild.member(user);
			if (member) {
				member.kick('Дополнительная причина, которая будет отображаться в журналах аудита').then(() => {
					message.reply(`Успешно кикнут ${user.tag}`);
				}).catch(err => {
					message.reply('Я не могу кикнуть этого пользователя!');
					console.error(err);
				});
			} else {
				message.reply('Этот пользователь не находиться на этом канале');
			}
		} else {
			message.reply('Вы не упомянули, что пользователь кикнут! Пример: @user');
		}
	}

	if (message.content.startsWith('Привет')) {
		message.react('✌')
			.then(console.log)
			.catch(console.error);
	    message.react('😉')
			.then(console.log)
			.catch(console.error);
	}

	if (message.content === "!clear"){
        async function clear() {
            message.delete()
		    .then(message => console.log(`Deleted from ${message.author.username}`))
		    .catch(console.error);
            const fetched = await message.channel.fetchMessages({limit: 99});
            message.channel.bulkDelete(fetched);
        }
        clear();
	}

	if (message.content === "!help") {
		const embed = new RichEmbed()
			.setTitle('Справка по командам:')
			.setColor(0xFF1122)
			.setDescription('1.!myavatar - показывает ваш аватар.\n2.Что-то еще\n');
		message.channel.send(embed);
    }

	if (message.content === '!myavatar') {
		message.reply(message.author.avatarURL);
	}

});

client.login("NjAxNzY4MzIwMjUzMDM0NDk2.XTHHuQ.I1xV5yFOhk3B25gywUVVh9RigS0"); // токен