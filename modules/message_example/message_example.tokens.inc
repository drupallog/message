<?php

/**
 * @file
 * Place holder for the message example module.
 */

use Drupal\Component\Utility\String;
use Drupal\Component\Utility\Xss;

/**
 * Implements hook_token_info().
 *
 * We need this part until the token module will be exported to Drupal 8.
 */
function message_example_token_info() {
  $type = array(
    'name' => t('Message example'),
    'description' => t('Tokens for the message example module.'),
    'needs-data' => 'message',
  );

  // User relate tokens.
  $message['user-name'] = array(
    'name' => t("Author name"),
    'description' => t("The related author name."),
  );

  $message['user-url'] = array(
    'name' => t("Author url"),
    'description' => t("The related author URL."),
  );

  // Comment relate tokens.
  $message['comment-url'] = array(
    'name' => t("Comment url"),
    'description' => t("The comment url."),
  );

  // Node related tokens.
  $message['node-render'] = array(
    'name' => t("Rendered node"),
    'description' => t("The related node rendered to HTML."),
  );

  $message['node-title'] = array(
    'name' => t("Node title"),
    'description' => t("The related node title."),
  );

  $message['node-url'] = array(
    'name' => t("Node URL"),
    'description' => t("The related node URL."),
  );

  return array(
    'types' => array('message' => $type),
    'tokens' => array(
      'message' => $message,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function message_example_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $token_service = \Drupal::token();

  $url_options = array('absolute' => TRUE);
  if (isset($options['langcode'])) {
    $url_options['language'] = \Drupal::languageManager()->getLanguage($options['langcode']);
    $langcode = $options['langcode'];
  }
  else {
    $langcode = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'message' && !empty($data['message'])) {
    /** @var \Drupal\message\Entity\Message $message */
    $message = $data['message'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the comment.
        case 'user-name':
          $replacements[$original] = $message->getAuthor()->label();
          break;
        case 'user-url':
          if ($message->getAuthor()) {
            $replacements[$original] = $message->getAuthor()->url();
          }
          break;
        case 'node-title':
            if ($message->bundle() == 'example_create_comment') {
              $replacements[$original] = $message->get('field_comment_reference')->getEntity()->label();
            }
            else {
              $replacements[$original] = $message->get('field_node_reference')->getEntity()->label();
            }
          break;
        case 'node-url':
          if ($message->bundle() == 'example_create_comment') {
            $replacements[$original] =$message->get('field_comment_reference')->getEntity()->toUrl()->toString();
          }
          else {
            $replacements[$original] = $message->get('field_node_reference')->getEntity()->toUrl()->toString();
          }
          break;
        case 'node-render':
          if ($message->bundle() == 'example_create_comment') {
            $view_builder = \Drupal::entityTypeManager()->getViewBuilder($message->getEntityTypeId());
            $replacements[$original] = render($view_builder->view($message->get('field_comment_reference')->getEntity(), 'teaser', $langcode));
          }
          else {
            $view_builder = \Drupal::entityTypeManager()->getViewBuilder($message->getEntityTypeId());
            $replacements[$original] = render($view_builder->view($message->get('field_node_reference')->getEntity(), 'teaser', $langcode));
          }
          break;
        case 'comment-url':
          $replacements[$original] = $message->get('field_comment_reference')->getEntity()->id();
          break;
      }
    }
  }

  return $replacements;
}
