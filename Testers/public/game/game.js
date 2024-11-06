// --------------------------------------------
// CONFIG
// --------------------------------------------
const paddleSpeed = 20;
const ballSpeed = 2;
const upP1 = 'w';
const dnP1 = 's';
const upP2 = 'ArrowUp';
const dnp2 = 'ArrowDown';
// --------------------------------------------
// GETTER & SETTER
// --------------------------------------------
const game = document.getElementById('game');
const gameRect = game.getBoundingClientRect();
const PL = document.getElementById('paddle-left');
const PR = document.getElementById('paddle-right');
const ball = document.getElementById('ball');
let ballX = game.clientWidth / 2;
let ballY = game.clientHeight / 2;
let ballVelocityX = ballSpeed;
let ballVelocityY = ballSpeed;
// --------------------------------------------
// FUNCTIONS
// --------------------------------------------

// LIAW-NOTE [SUN-29TH-SEP-2024]:
// 1. this function sets the paddle movement using keydown event
// 1. it checks if it over the top or bottom of the game area
//    before moving the paddle
function setPaddle()
{
	document.addEventListener('keydown', function (event) {
		const PLRect = PL.getBoundingClientRect();
		const PRRect = PR.getBoundingClientRect();

		// Left paddle
		if (event.key === upP1)
		{
			if (PLRect.top > game.getBoundingClientRect().top)
				PL.style.top = PL.offsetTop - paddleSpeed + 'px';
		}
		else if (event.key === dnP1)
		{
			if (PLRect.bottom < game.getBoundingClientRect().bottom)
				PL.style.top = PL.offsetTop + paddleSpeed + 'px';
		}

		// Right paddle
		if (event.key === upP2)
		{
			if (PRRect.top > game.getBoundingClientRect().top)
				PR.style.top = PR.offsetTop - paddleSpeed + 'px';
		}
		else if (event.key === dnp2)
		{
			if (PRRect.bottom < game.getBoundingClientRect().bottom)
				PR.style.top = PR.offsetTop + paddleSpeed + 'px';
		}
	});
}

// LIAW-NOTE [SUN-29TH-SEP-2024]:
// 1. this function keep the ball bouncing against items
// 1. bounce & collision  logic : github-jakesgordon, so and chatgpt
// 1. my code may not be the best way to apply the logic of them
// 1. but it works for now; serving the draft purpose of the game
// 1. it simply checks if it reaches the top/botom wall or paddles
//    then it changes the direction of the ball (ballVelocityX/Y);
//    from moving left/right or up/down to right/left or down/up
// 1. note : ballX and ballY are the center of the ball
// 1. note : this bounce and collision hasn't respect the 
//           game logic of the original pong game yet due to the
//           bounce physics is not yet implemented
function moveBall()
{
	ballX += ballVelocityX;
	ballY += ballVelocityY;
	const ballRect = ball.getBoundingClientRect();
	const PLRect = PL.getBoundingClientRect();
	const PRRect = PR.getBoundingClientRect();

	// Ball bounce &collision
	// - top/bottom walls
	if (ballY <= 0 || (ballY + ballRect.height / 2) >= gameRect.height)
		ballVelocityY *= -1;
	//if (ballY <= 0 || ballY + ball.clientHeight >= game.clientHeight)

	// - left paddle
	if (ballX <= PLRect.width
		&& ballY >= PL.offsetTop
		&& ballY <= PL.offsetTop + PLRect.height)
		ballVelocityX *= -1;

	// - right paddle
	if (ballX + ballRect.width / 2 >= gameRect.width - PRRect.width
		&& ballY >= PR.offsetTop
		&& ballY <= PR.offsetTop + PRRect.height)
		ballVelocityX *= -1;

	// [ game over]
	// - left/right walls; reset to center
	if (ballX <= 0 || (ballX + ballRect.width / 2) >= gameRect.width)
	{
		ballX = game.clientWidth / 2;
		ballY = game.clientHeight / 2;
		ballVelocityX *= -1;
	}

	// update the css
	ball.style.left = ballX + 'px';
	ball.style.top = ballY + 'px';

	// Repeat
	requestAnimationFrame(moveBall);
}

(function () {
	requestAnimationFrame(moveBall);
	setPaddle();
})();
