const https = require('https');

function generateInviteCode(size = 7, chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQrstuvwxyz0123456789'.split('')) {
  const code = Array(size).fill().map(() => chars[Math.floor(Math.random() * chars.length)]).join('');
  return code;
}

function checkInvite(code) {
  return new Promise((resolve, reject) => {
    const req = https.get(`https://discordapp.com/api/v6/invites/${code}`, res => {
      let data = '';
      res.on('data', chunk => {
        data += chunk;
      });
      res.on('end', () => {
        if (res.statusCode === 200) {
          console.log(`Valid invite code: ${code}`); // Logging the valid code
          resolve(JSON.parse(data));
        } else {
          resolve(null);
        }
      });
    }).on('error', err => {
      reject(err);
    });

    req.on('end', () => {
      if (!req.response) {
        console.log(`No response for invite code: ${code}`); // Logging the case where there is no response
      }
    });
  });
}

async function main() {
  while (true) {
    const codes = Array(4).fill().map(() => generateInviteCode());
    const results = await Promise.all(codes.map(checkInvite));
    results.forEach((server, index) => {
      if (server) {
        console.log(`Found valid invite: ${codes[index]} for server '${server.guild.name}'`); // Logging the valid invite
      }
    });
  }
}

main();
