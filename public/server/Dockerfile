FROM node:16

# Create app directory
WORKDIR /usr/src/app

# Install app dependacies
# A wildcard is used to ensure both package.json AND package-lock.json are copied where available (npm@5+)
COPY package*.json ./

RUN npm install
# If you are building your code for production
# RUN npm ci --only=production

# Bundle app source
COPY . .

EXPOSE 7777
CMD [ "npm", "run", "start" ]

# Build the server
# docker build . -t myserver

# Port mapping
# docker run -p 1111:7777 -d myserver