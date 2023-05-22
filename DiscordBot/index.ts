import DiscordJS, { Intents, MessageActionRow } from "discord.js"
import dotenv from "dotenv"

dotenv.config()

const client = new DiscordJS.Client({
    intents:[
        Intents.FLAGS.GUILDS,
        Intents.FLAGS.GUILD_MESSAGES,
        Intents.FLAGS.DIRECT_MESSAGES,
        Intents.FLAGS.DIRECT_MESSAGE_REACTIONS,

    ]
})

client.on("ready", ()=>{
    console.log("The bot is ready!");

    const guildId = "966029683814703124"
    const guild =client.guilds.cache.get(guildId)
    let commands
    if(guild){
    commands = guild.commands

    } else{ 
        commands = client.application?.commands
    }
   

    
});

client.on("interactionCreate", async (interaction) => {
    if(!interaction.isCommand()){
        return 
    }
    const{commandName, options} = interaction

    if(commandName === "ping"){
        interaction.reply({
            content:"pong",
            ephemeral: true
        })
    }
})



client.on("messageCreate", (message)=>{
    if(message.content == "?chalga"){
        message.reply({
            content: "Chalga mix 2022(Tempo) >play https://open.spotify.com/playlist/5A7fiwYiPbbz0ClH5j2yb9?si=EV0RZ3FmQ-6w4kjA3eYcdg&utm_source=copy-link\n Retro 5(Tempo) >play https://open.spotify.com/playlist/1FZyMW99HK6MoGBHE10U0g?si=67a49725827d4d84 \n Mix2020(Vexera) +play https://www.youtube.com/watch?v=fotIk_Bv1AI&t=5620s"
    })}
});

client.on("messageCreate", (message)=>{
    if(message.content == "?techno"){
        message.reply({
            content: "Exe Mix(Tempo) >play https://open.spotify.com/playlist/5A7fiwYiPbbz0ClH5j2yb9?si=EV0RZ3FmQ-6w4kjA3eYcdg&utm_source=copy-link\n House(TEMPO) >play https://open.spotify.com/playlist/6xngKuUgWATB0cMPFlvAgS?si=4aed85b3803549f1"
        })
    }
})
client.on("messageDelete", (message)=>{
        if (message.content == "ping"){
        message.reply({
            content: "Swear was deleted"
        
    })}
})

client.on("messageCreate", (message)=>{
    if(message.content == "nigga"){
        message.delete()
        message.reply({
            content: "Swear was deleted"	

        })
       
    }
})



client.login(process.env.TOKEN)