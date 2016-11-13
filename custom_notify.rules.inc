<?php

/**
 * Implements hook_rules_action_info().
 */
function custom_notify_rules_action_info() {
  $actions = array();
  $actions['custom_notify_telegram'] = array(
    'label' => t('Send telegram messages'),
    'group' => t('Notification'),
    'parameter' => array(
      'tg_token' => array(
        'type' => 'text',
        'optional' => FALSE,
        'lable' => t('Access token'),
        'restriction' => 'input',
        'description' => t('Token to access the HTTP API')
      ),
      'tg_channel' => array(
        'type' => 'text',
        'lable' => t('Bot channel'),
        'optional' => FALSE,
        'restriction' => 'input',
        'description' => t('Pablic channel name (name after https://telegram.me/ with starting @. For sample @test_chat)')
      ),
      'tg_message' => array(
        'type' => 'text',
        'label' => t('Message'),
        'description' => t("The content of the message."),
      ),
    ),
  );
  return $actions;

}

function custom_notify_telegram($tg_token, $tg_channel, $tg_message) {
  if (!function_exists('curl_init')) {
    watchdog('custom_notify','Ошибка curl: curl недоступен');
    return false;
  };
  $tg_message = urlencode($tg_message);
  // составляем запрос по всем правилам Telegram api
  $query = "https://api.telegram.org/bot" . $tg_token .
    "/sendMessage?disable_web_page_preview=true&chat_id=" . $tg_channel .
    "&text=" . $tg_message;

  // выполняем наш запрос через curl
  $curl = curl_init();
  curl_setopt($curl, CURLOPT_URL, $query);
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($curl, CURLOPT_HEADER, 0);
  if(curl_exec($curl) === false)
  {
    watchdog('custom_notify','Ошибка curl: ' . curl_error($curl));
  }
  else
  {
    watchdog('custom_notify', 'Сообщение в чат ' . $tg_channel . ' отправлено');
  }

  curl_close($curl);
}