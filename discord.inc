#if defined _discord_included
	#endinput
#endif
#define _discord_included

typedef DiscordGuildsRetrieve = function void (DiscordBot bot, char[] id, char[] name, char[] icon, bool owner, int permissions, any data);

typedef DiscordGuildsRetrievedAll = function void (DiscordBot bot, ArrayList id, ArrayList name, ArrayList icon, ArrayList owner, ArrayList permissions, any data);

//Channel are Handles that are closed immediately after forwards called. To keep, clone. Or store id if thats what you want
typedef DiscordGuildChannelsRetrieve = function void (DiscordBot bot, char[] guild, DiscordChannel Channel, any data);

typedef DiscordGuildChannelsRetrieveAll = function void (DiscordBot bot, char[] guild, ArrayList Channels, any data);

typeset OnChannelMessage { 
	function void(DiscordBot bot, DiscordChannel channel, const char[] message);
	function void(DiscordBot bot, DiscordChannel channel, const char[] message, const char[] messageID);
	function void(DiscordBot bot, DiscordChannel channel, const char[] message, const char[] messageID, const char[] userID);
	function void(DiscordBot bot, DiscordChannel channel, const char[] message, const char[] messageID, const char[] userID, const char[] userName, const char[] discriminator);
	function void(DiscordBot bot, DiscordChannel channel, const char[] message, const char[] messageID, const char[] userID, const char[] userName, const char[] discriminator, Handle hJson);
};

methodmap DiscordChannel < StringMap {
	public DiscordChannel() {
		StringMap mp = new StringMap();
		return view_as<DiscordChannel>(mp);
	}
	
	public native void SendMessage(DiscordBot Bot, char[] message);

	public void GetGuildID(char[] buffer, int maxlength) {
		GetTrieString(this, "guild_id", buffer, maxlength);
	}
	
	public void GetID(char[] buffer, int maxlength) {
		GetTrieString(this, "id", buffer, maxlength);
	}
	
	public void GetName(char[] buffer, int maxlength) {
		GetTrieString(this, "name", buffer, maxlength);
	}
	
	public void GetType(char[] buffer, int maxlength) {
		GetTrieString(this, "type", buffer, maxlength);
	}
	
	property int Position {
		public get() {
			int value;
			GetTrieValue(this, "position", value);
			return value;
		}
	}
	
	property bool IsPrivate {
		public get() {
			int value;
			GetTrieValue(this, "is_private", value);
			return view_as<bool>(value);
		}
	}
	
	public void GetTopic(char[] buffer, int maxlength) {
		GetTrieString(this, "topic", buffer, maxlength);
	}
	
	public void GetLastMessageID(char[] buffer, int maxlength) {
		GetTrieString(this, "last_message_id", buffer, maxlength);
	}
	
	public void SetLastMessageID(char[] id) {
		SetTrieString(this, "last_message_id", id);
	}
	
	property int Bitrate {
		public get() {
			int value;
			GetTrieValue(this, "bitrate", value);
			return value;
		}
	}
	
	property int UserLimit {
		public get() {
			int value;
			GetTrieValue(this, "user_limit", value);
			return value;
		}
	}
	
	property bool IsText {
		public get() {
			char type[8];
			this.GetType(type, sizeof(type));
			if(StrEqual(type, "text", false)) return true;
			return false;
		}
	}
	
	property Handle MessageCallback {
		public get() {
			Handle fForward = null;
			if(!GetTrieValue(this, "callback", fForward)) {
				return null;
			}
			
			return fForward;
		}
		
		public set(Handle value) {
			Handle fForward = null;
			if(GetTrieValue(this, "callback", fForward)) {
				delete fForward;
			}
			
			SetTrieValue(this, "callback", value);
		}
	}
}

methodmap DiscordBot < StringMap {
	public DiscordBot(const char[] token) {
		StringMap mp = new StringMap();
		SetTrieString(mp, "token", token);
		return view_as<DiscordBot>(mp);
	}
	
	property float MessageCheckInterval {
		public get() {
			float val;
			if(!GetTrieValue(this, "messageinterval", val)) {
				return 1.0;
			}
			return val;
		}
		public set(float value) {
			SetTrieValue(this, "messageinterval", value);
		}
	}
	
	public native void StartTimer(DiscordChannel Channel);
	
	/** 
	 * Retrieves a list of Channels the bot is listening to for messages
	 */
	public ArrayList GetListeningChannels() {
		ArrayList alChannels = null;
		GetTrieValue(this, "channels", alChannels);
		return alChannels;
	}
	
	/**
	 * Checks if the bot is listening to channel for messages
	 * @param DiscordChannel Channel
	 */
	public bool IsListeningToChannel(DiscordChannel Channel) {
		char id[32];
		Channel.GetID(id, sizeof(id));
		
		ArrayList channels = this.GetListeningChannels();
		if(channels == null) return false;
		
		for(int i = 0; i < channels.Length; i++) {
			DiscordChannel tempChannel = channels.Get(i);
			static char tempID[32];
			tempChannel.GetID(tempID, sizeof(tempID));
			if(StrEqual(id, tempID, false)) {
				//delete channels;
				return true;
			}
		}
		//delete channels;
		return false;
	}
	
	/**
	 * Checks if the bot is listening to channel for messages
	 * @param DiscordChannel Channel
	 */
	public bool IsListeningToChannelID(char[] id) {
		ArrayList channels = this.GetListeningChannels();
		if(channels == null) return false;
		
		for(int i = 0; i < channels.Length; i++) {
			DiscordChannel tempChannel = channels.Get(i);
			static char tempID[32];
			tempChannel.GetID(tempID, sizeof(tempID));
			if(StrEqual(id, tempID, false)) {
				//delete channels;
				return true;
			}
		}
		//delete channels;
		return false;
	}
	
	/**
	 * Stops the bot from listening to that channel for messages
	 * @param DiscordChannel Channel
	 */
	public void StopListeningToChannel(DiscordChannel Channel) {
		char id[32];
		Channel.GetID(id, sizeof(id));
		
		ArrayList channels = this.GetListeningChannels();
		if(channels == null) return;
		
		for(int i = 0; i < channels.Length; i++) {
			DiscordChannel tempChannel = channels.Get(i);
			static char tempID[32];
			tempChannel.GetID(tempID, sizeof(tempID));
			if(StrEqual(id, tempID, false)) {
				channels.Erase(i);
				delete tempChannel;
				
				if(tempChannel.MessageCallback != null) {
					delete tempChannel.MessageCallback;
					tempChannel.MessageCallback = null;
				}
			}
		}
		//delete channels;
	}
	
	/**
	 * Stops the bot from listening to that channel id for messages
	 * @param DiscordChannel Channel
	 */
	public void StopListeningToChannelID(char[] ChannelID) {
		ArrayList channels = this.GetListeningChannels();
		
		if(channels == null) return;
		
		for(int i = 0; i < channels.Length; i++) {
			DiscordChannel tempChannel = channels.Get(i);
			static char tempID[32];
			tempChannel.GetID(tempID, sizeof(tempID));
			if(StrEqual(ChannelID, tempID, false)) {
				channels.Erase(i);
				delete tempChannel;
				
				if(tempChannel.MessageCallback != null) {
					delete tempChannel.MessageCallback;
					tempChannel.MessageCallback = null;
				}
			}
		}
		//delete channels;
	}
	
	public DiscordChannel GetListeningChannelByID(char[] ChannelID) {
		ArrayList channels = this.GetListeningChannels();
		if(channels == null) return null;
		for(int i = 0; i < channels.Length; i++) {
			DiscordChannel tempChannel = channels.Get(i);
			static char tempID[32];
			tempChannel.GetID(tempID, sizeof(tempID));
			if(StrEqual(ChannelID, tempID, false)) {
				//delete channels;
				return tempChannel;
			}
		}
		return null;
	}
	
	/**
	 * Start listening to the channel for messages.
	 * The Channel handle is duplicated. Feel free to close yours.
	 * @param DiscordChannel Channel
	 */
	public void StartListeningToChannel(DiscordChannel Channel, OnChannelMessage callback) {
		if(this.IsListeningToChannel(Channel)) return;
		
		DiscordChannel newChannel = view_as<DiscordChannel>(CloneHandle(Channel));
		ArrayList alChannels = this.GetListeningChannels();
		
		if(alChannels == null) {
			alChannels = new ArrayList();
			SetTrieValue(this, "channels", alChannels);
		}
		
		alChannels.Push(newChannel);
		//delete alChannels;
		
		Handle fForward = CreateForward(ET_Ignore, Param_Cell, Param_Cell, Param_String, Param_String, Param_String, Param_String, Param_String, Param_Cell);
		AddToForward(fForward, GetMyHandle(), callback);
		newChannel.MessageCallback = fForward;
		
		this.StartTimer(newChannel);
	}
	
	public native void GetToken(char[] token, int maxlength);
	
	public native void SendMessage(DiscordChannel channel, char[] message);
	
	public native void SendMessageToChannelID(char[] channel, char[] message);
	
	public native void GetGuilds(DiscordGuildsRetrieve fCallback = INVALID_FUNCTION, DiscordGuildsRetrievedAll fCallbackAll = INVALID_FUNCTION, any data=0);
	
	public native void GetGuildChannels(char[] guild, DiscordGuildChannelsRetrieve fCallback = INVALID_FUNCTION, DiscordGuildChannelsRetrieveAll fCallbackAll = INVALID_FUNCTION, any data=0);
}